#include "frompdf.h"

#ifdef PDFSUPPORT
#include "../utils/dialog_critic/dialog_critic.h"
#include "../currenttitle/document.h"
#include "../utils/permission/permission.h"
#include "../dataread/xmlstruct.h"
#include "../dataread/load_from_file.h"
#include "../dataread/readlistarray.h"
#include "../datawrite/savefile.h"
#include <QFileDialog>
#include "../dataread/load_from_file.h"
#include "../utils/areyousure/areyousure.h"
#include "convertImg.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "../touch/tabletcanvas.h"
#include "../utils/threadcount.h"

void frompdf::translation(const QPointF &point)
{
    int i;
    if(!m_data->count_pdf)
        return;

    for(i=0; i<this->m_image.length(); ++i){
        this->m_image.operator[](i).topLeft += point;
    }
}

frompdf::frompdf(Document *data)
{
    m_data = data;
}

bool frompdf::load(const QStringList &path, QMap<load_res, uchar> &index, TabletCanvas *canvas)
{
    uint i, len;
    load_res __r;

    index.clear();
    reset();

    len = path.length();

    for(i=0; i<len; ++i){
        __r = load(path.at(i), false, canvas);
        if(__r != load_res::ok){
            index.insert(__r, i);
        }
    }

    return !index.isEmpty();
}

frompdf::load_res frompdf::load(const QString &path, const bool clear, TabletCanvas *canvas)
{
    QByteArray arr;

    if(!load_from_file::exe(arr, path, false)){
        return load_res::not_valid_pdf;
    }

    /*
     *  if we do the load with this function is always the firstLost
    */
    return load_from_row(arr, clear, true, 0, canvas);
}

QStringList frompdf::get_name_pdf(){
    uint i;
    QStringList __l;
    for(i=0; i<m_data->count_pdf; ++i){
        __l.append(frompdf::getName(i));
    }

    return __l;
}

frompdf::load_res frompdf::load(zip_t *fileZip,
                                zip_file_t *file,
                                TabletCanvas *canvas)
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
                                 file == nullptr,
                                 i,
                                 canvas);
        if(res != frompdf::load_res::ok)
            return res;
    }

    return frompdf::load_res::ok;

}

void frompdf::resizing(TabletCanvas *canvas, const uint lenPdf){
    if(!canvas)
        return;

    uint i = m_data->datatouch->num_page();
    for(; i<lenPdf; ++i){
        canvas->disegnofoglio_bool = true;
        canvas->disegnafoglio();
    }
}

frompdf::load_res frompdf::load_from_row(const QByteArray &pos, const bool clear,
                                         const bool FirstLoad, const uchar IndexPdf,
                                         TabletCanvas *canvas)
{
    /*
     * in the current version we cannot upload more
     * than one pdf at the same time
    */

    uint i, len;
    QImage imgAppend;
    QList<Poppler::Page *> page;
    QList<convertImg *> conv;
    uint pdfCount;
    const uint countThread = threadCount::count();

    if(clear)
        this->reset();

    if(FirstLoad){
        this->init_FirstLoad();
    }

    doc = Poppler::Document::loadFromData(pos);

    if(!doc){
        if(areyousure("Pdf error loading", "it seems the pdf file is correct, do you want to remove it?")){
            this->m_data->count_pdf = 0;
            return load_res::ok;
        }
        return load_res::not_valid_pdf;
    }

    len = doc->numPages();
    this->resizing(canvas, len);

    for(i=0; i<len && i<countThread; ++i){
        page.append(doc->page(QString::number(i+1)));
        this->m_image.operator[](IndexPdf).img.append(imgAppend);
        conv.append(new convertImg(resolution));
    }

    for(pdfCount = 0; pdfCount < len; ){
        for(i=0; i < countThread && i < len; ++i){
            const auto pdfPage = page.at(pdfCount);
            QImage *image = &m_image.operator[](IndexPdf).img.operator[](pdfCount);
            conv.operator[](i)->setData(pdfPage, image);
            conv.at(i)->start();
        }

        for(i=0; i < countThread && i < len; ++i){
            conv.at(i)->wait();
        }

        pdfCount = (pdfCount+1) * len;
    }
    for(i=0; i<(uint)page.length(); ++i)
        delete page.at(i);
    for(i=0; i<(uint)conv.length(); ++i)
        delete conv.at(i);


    for(i=0; i<this->m_image.at(IndexPdf).img.at(i).isNull(); ++i){
        dialog_critic("We had a problem processing an image");
        return load_res::not_valid_pdf;
    }

    if(FirstLoad)
        this->adjast(IndexPdf);

    if(canvas)
        canvas->update();

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
                                   path_writernote_file,
                                   filezip,
                                   frompdf::getName(m_data->count_pdf),
                                   false) != OK)
        return load_res::not_valid_pdf;

    return load_res::ok;
}

/*
 * add image from position
*/
void frompdf::addPdf(QString &pos,
                     const PointSettable *point,
                     const QString &path_writernote,
                     TabletCanvas *canvas)
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

    m_data->datatouch->scala_all();
    if(insert_pdf(pos, point) != OK){
        res = load_res::not_valid_pdf;
        goto err;
    }

    res = this->save(fileZip, pos, path_writernote);
    if(res != load_res::ok)
        goto err;

    this->m_data->count_pdf ++;


    /* in order to upload the pdf file we have to reopen the zip file */
    zip_close(fileZip);
    fileZip = zip_open(path_writernote, ZIP_CREATE, &ok);

    res = this->load(fileZip, nullptr, canvas);

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
    m_image.operator[](indexPdf).topLeft = QPointF(0, 0);
}

uchar frompdf::insert_pdf(QString &pos,
                          const PointSettable *point)
{
    assert(this->m_image.length() == 0);
    Pdf pdf;

    if(pos == ""){
        pos = QFileDialog::getOpenFileName(nullptr,
                                                     "Open images",
                                                     "JPEG (*.jpg, *.jpeg);;PNG (*.png);;All Files (*)");

        if(pos == "")
            return ERROR;
    }

    if(point){
        pdf.topLeft = point->point;
    }
    else{
        pdf.topLeft = QPointF(0.0, 0.0);
    }

    this->m_image.append(pdf);

    return OK;
}

#endif
