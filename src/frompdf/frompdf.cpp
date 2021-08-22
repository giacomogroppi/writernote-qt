#include "frompdf.h"

#include "../utils/dialog_critic/dialog_critic.h"
#include "../currenttitle/document.h"
#include "../utils/permission/permission.h"
#include "../dataread/xmlstruct.h"
#include "../dataread/load_from_file.h"
#include "../dataread/readlistarray.h"
#include "../datawrite/savefile.h"
#include <QFileDialog>

void frompdf::translation(const double x, const double y)
{
    if(!m_data->count_pdf)
        return;

    m_translation.x1 += x;
    m_translation.y1 += y;
    m_translation.x2 += x;
    m_translation.y2 += y;
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

    return load_from_row(arr, clear);
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
                                 file != nullptr);
        if(res != frompdf::load_res::ok)
            return res;
    }

    return frompdf::load_res::ok;

}

frompdf::load_res frompdf::load_from_row(const QByteArray &pos, const bool clear, const bool FirstLoad)
{
    uint i, len;
    QImage img;

    if(clear)
        this->reset();

    doc = Poppler::Document::loadFromData(pos);

    if(!doc){
        dialog_critic("The file is not readable");
        return load_res::not_valid_pdf;
    }

    len = doc->numPages();

    for(i=0; i<len; ++i){
        img = doc->page(QString::number(i))->renderToImage(
                                    5 * IMG_PDF_HEIGHT,
                                    5 * IMG_PDF_HEIGHT);

        if(img.isNull())
            return load_res::not_valid_page;

        m_image.operator[](i).immagini = img;

    }

    if(FirstLoad)
        this->adjast();

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
    if(savefile::saveArrayIntoFile(path,
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
    if(insert_pdf(pos, point) != OK)
        goto err;

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
}

void frompdf::adjast()
{
    int i, height;

    height = 0;

    for(i=0; i<this->m_image.length(); ++i){
        m_image.operator[](i).i = QPointF(height, NUMEROPIXELPAGINA);
        m_image.operator[](i).f = QPointF(height + NUMEROPIXELORIZZONALI, NUMEROPIXELPAGINA);

        height += NUMEROPIXELORIZZONALI;
    }

}

uchar frompdf::insert_pdf(QString &pos,
                          const PointSettable *point)
{
    if(pos == ""){
        pos = QFileDialog::getOpenFileName(nullptr,
                                                     "Open images",
                                                     "JPEG (*.jpg, *.jpeg);;PNG (*.png);;All Files (*)");

        if(pos == "")
            return ERROR;
    }

    if(point){
        m_translation.x1 = point->point.toPoint().x();
        m_translation.y1 = point->point.toPoint().y();

        m_translation.x2 = point->point.toPoint().x() + double(NUMEROPIXELPAGINA);
        m_translation.y2 = point->point.toPoint().y() + double(NUMEROPIXELORIZZONALI);
    }
    else{
        m_translation.x1 = m_translation.y1 = double(0);

        m_translation.x2 = double(NUMEROPIXELPAGINA);
        m_translation.y2 = double(NUMEROPIXELORIZZONALI);
    }

    return OK;
}
