#include "frompdf.h"

#include "../utils/dialog_critic/dialog_critic.h"
#include "../currenttitle/document.h"
#include "../utils/permission/permission.h"
#include "../dataread/xmlstruct.h"
#include "../dataread/load_from_file.h"
#include "../dataread/readlistarray.h"
#include "../datawrite/savefile.h"
#include <QFileDialog>
#include "../dataread/load_from_file.h"

void frompdf::translation(const QPointF &point)
{
    int i;
    if(!m_data->count_pdf)
        return;

    for(i=0; i<this->m_image.length(); ++i){
        this->m_image.operator[](i).topLeft += point;
        this->m_image.operator[](i).bottomRigth += point;
    }

}

frompdf::frompdf(Document *data)
{
    m_data = data;
}

bool frompdf::load(const QStringList &path, QMap<load_res, uchar> &index)
{
    uint i, len;
    load_res __r;

    index.clear();
    reset();

    len = path.length();

    for(i=0; i<len; ++i){
        __r = load(path.at(i), false);
        if(__r != load_res::ok){
            index.insert(__r, i);
        }
    }

    return !index.isEmpty();
}

frompdf::load_res frompdf::load(const QString &path, const bool clear)
{
    QByteArray arr;

    if(!load_from_file::exe(arr, path, false)){
        return load_res::not_valid_pdf;
    }

    /*
     *  if we do the load with this function is always the firstLost
    */
    return load_from_row(arr, clear, true, 0);
}

QStringList frompdf::get_name_pdf(){
    uint i;
    QStringList __l;
    for(i=0; i<m_data->count_pdf; ++i){
        __l.append(frompdf::getName(m_data->nome_copybook, i));
    }

    return __l;
}

frompdf::load_res frompdf::load(zip_t *fileZip, zip_file_t *file)
{
    QList<QByteArray> arr;
    QStringList __name;
    uint i;

    __name = get_name_pdf();

    if(file && this->load_metadata(file) != load_res::ok){
        return load_res::no_metadata;
    }

    if(readListArray::read(__name, fileZip, arr, false) != OK){
        return frompdf::load_res::not_valid_pdf;
    }

    for (i=0; i<m_data->count_pdf; ++i){
        auto res = load_from_row(arr.at(i),
                                 false,
                                 file != nullptr,
                                 i);
        if(res != frompdf::load_res::ok)
            return res;
    }

    return frompdf::load_res::ok;

}

frompdf::load_res frompdf::load_from_row(const QByteArray &pos, const bool clear,
                                         const bool FirstLoad, const uchar IndexPdf)
{
    /*
     * in the current version we cannot upload more
     * than one pdf at the same time
    */

    uint i, len;
    QImage img;
    struct immagine_s imgAppend;

    if(clear)
        this->reset();

    if(FirstLoad){
        this->init_FirstLoad();
    }

    doc = Poppler::Document::loadFromData(pos);

    if(!doc){
        dialog_critic("The file is not readable");
        return load_res::not_valid_pdf;
    }

    len = doc->numPages();

    for(i=0; i<len; ++i){
        img = doc->page(QString::number(i+1))->renderToImage(
                                    5 * IMG_PDF_HEIGHT,
                                    5 * IMG_PDF_HEIGHT);

        if(img.isNull())
            return load_res::not_valid_page;

        imgAppend.immagini = img;

        this->m_image.operator[](IndexPdf).img.append(imgAppend);

    }

    if(FirstLoad)
        this->adjast(IndexPdf);

    return load_res::ok;
}

frompdf::load_res frompdf::save(zip_t *filezip,
                                const QStringList &path,
                                const QString &path_writernote_file)
{
    frompdf::load_res res;
    uint i, len;

    this->m_data->count_pdf = path.length();
    len = this->m_data->count_pdf;

    for(i=0; i<len; ++i){
        res = this->save(filezip,
                         path.at(i),
                         path_writernote_file);
        if(res != frompdf::load_res::ok)
            return res;
    }
    return frompdf::load_res::ok;
}

frompdf::load_res frompdf::save(zip_t *filezip,
                                const QString &path,
                                const QString &path_writernote_file)
{
    if(savefile::saveArrayIntoFile((const QString &)path,
                                   this->m_data->nome_copybook,
                                   path_writernote_file,
                                   filezip,
                                   frompdf::getNameNoCopy(m_data->count_pdf),
                                   false) != OK)
        return load_res::not_valid_pdf;

    return load_res::ok;
}

/*
 * add image from position
*/
void frompdf::addPdf(QString &pos,
                         const PointSettable *point,
                         const QString &path_writernote)
{
    zip_t *fileZip;
    int ok;
    frompdf::load_res res;

    fileZip = zip_open(path_writernote, ZIP_CREATE, &ok);
    if(!fileZip){
        res = frompdf::load_res::no_valid_path;
        goto err;
    }


    if(path_writernote == "")
        return dialog_critic("Before add a pdf you need to save this file");
    if(m_data->count_pdf)
        return user_message("It's not possible to add more than one pdf");
    if(!m_data->m_touch)
        return user_message("It's not possible to pdf in keyboard mode");

    m_data->datatouch->scala_all();
    if(insert_pdf(pos, point) != OK){
        res = load_res::not_valid_pdf;
        goto err;
    }

    res = this->save(fileZip, pos, path_writernote);
    if(res != load_res::ok)
        goto err;

    this->m_data->count_pdf ++;

    this->load(fileZip, nullptr);

    err:
    if(res == load_res::no_valid_path)
        dialog_critic("We had trouble opening " + path_writernote);
    else if(res != load_res::ok)
        dialog_critic("We had some error");

    m_data->datatouch->restoreLastTranslation();
    zip_close(fileZip);
}

void frompdf::adjast(const uchar indexPdf)
{
    QPointF size = m_data->datatouch->get_size_page();
    assert(indexPdf > 0);

    m_image.operator[](indexPdf).topLeft = QPointF(0, 0);
    m_image.operator[](indexPdf).bottomRigth = size;

}

uchar frompdf::insert_pdf(QString &pos,
                          const PointSettable *point)
{
    assert(this->m_image.length() == 0);
    Pdf pdf;
    const QPointF size = this->m_data->datatouch->get_size_page();

    if(pos == ""){
        pos = QFileDialog::getOpenFileName(nullptr,
                                                     "Open images",
                                                     "JPEG (*.jpg, *.jpeg);;PNG (*.png);;All Files (*)");

        if(pos == "")
            return ERROR;
    }

    if(point){
        pdf.topLeft = point->point;
        pdf.bottomRigth = point->point + size;
    }
    else{
        pdf.topLeft = QPointF(0.0, 0.0);
        pdf.bottomRigth = size;
    }

    this->m_image.append(pdf);

    return OK;
}
