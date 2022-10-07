#include "frompdf.h"

#ifdef PDFSUPPORT
#include "utils/dialog_critic/dialog_critic.h"
#include "currenttitle/document.h"
#include "utils/permission/permission.h"
#include "dataread/xmlstruct.h"
#include "dataread/readlistarray.h"
#include "datawrite/savefile.h"
#include <QFileDialog>
#include "utils/areyousure/areyousure.h"
#include "convertImg.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "touch/tabletcanvas.h"
#include "utils/threadcount.h"
#include "core/WFile.h"

void frompdf::translation_pdf(const QPointF &point)
{
    for(auto &ref : m_image){
        ref.topLeft += point;
    }
}

frompdf::frompdf()
{
}

bool frompdf::load_pdf(
        const QList<QString>        &path,
        QMap<load_res_pdf, uchar>       &index,
        datastruct                  &data)
{
    uint i, len;
    load_res_pdf res;

    index.clear();
    reset_pdf();

    len = path.length();

    for(i=0; i<len; ++i){
        res = load_pdf(path.at(i), false, data);
        if(res != load_res_pdf::ok){
            index.insert(res, i);
        }
    }

    return !index.isEmpty();
}

frompdf::load_res_pdf frompdf::load_pdf(
        const QString   &path,
        cbool           clear,
        datastruct      &data)
{
    QByteArray arr;

    if(WFile::readFile(arr, path.toUtf8().constData()) < 0)
        return load_res_pdf::not_valid_pdf;

    /**
     *  if we do the load with this function is always the firstLost
    */
    return load_from_row_pdf(arr, clear, true, 0, data);
}

QList<QString> frompdf::get_name_pdf(int count)
{
    uint i;
    QList<QString> res;
    for(i = 0; i < count; i ++){
        res.append(frompdf::getName_pdf(i));
    }

    return res;
}

frompdf::load_res_pdf frompdf::load_pdf(const QByteArray &path_writernote_file, int len, datastruct &data)
{
    bool ok;
    WZip zip(path_writernote_file, ok);

    if(!ok){
        return frompdf::load_res_pdf::no_valid_path;
    }

    WZipReaderSingle reader(&zip, 0);

    return this->load_pdf(reader, len, data);
}

frompdf::load_res_pdf frompdf::load_pdf(WZipReaderSingle &reader, int len, datastruct &data)
{
    QList<QByteArray> arr;
    QList<QString> name;
    uint i;
    frompdf::load_res_pdf res;
    WZip *zip = reader.get_zip();

    m_image.clear();

    name = get_name_pdf(m_image.length());

    if( zip->is_data_available() and
        this->load_metadata_pdf(reader, len) != load_res_pdf::ok)
    {
        return load_res_pdf::no_metadata;
    }

    if(readListArray::read(name, *reader.get_zip(), arr, false) != OK)
        return frompdf::load_res_pdf::not_valid_pdf;

    for (i = 0; i < len; ++i){
        res = load_from_row_pdf(arr.at(i),
                                 false,
                                 !zip->is_data_available(),
                                 i,
                                 data);
        if(res != frompdf::load_res_pdf::ok)
            return res;
    }

    return frompdf::load_res_pdf::ok;
}

void frompdf::resizing_pdf(datastruct &data, int lenPdf)
{
    int i = data.lengthPage();
    for(; i < lenPdf; ++i){
        data.newPage(n_style::white);
    }
}

frompdf::load_res_pdf frompdf::load_from_row_pdf(
        const QByteArray    &pos,
        cbool               clear,
        cbool               FirstLoad,
        const uchar         IndexPdf,
        datastruct          &data)
{
    /**
     * in the current version we cannot upload more
     * than one pdf at the same time
    */

    int i, pdfCount;
    QImage imgAppend;

    W_ASSERT(IndexPdf == 0);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QList<Poppler::Page *> page;
#endif

    QList<convertImg *> conv;
    const int countThread = threadCount::count();

    if(clear)
        this->reset_pdf();

    if(FirstLoad){
        this->init_FirstLoad_pdf();
    }

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    std::unique_ptr<Poppler::Document> _doc = Poppler::Document::loadFromData(pos);
    const auto *doc = _doc.get();
#else
    const auto *doc = Poppler::Document::loadFromData(pos);
#endif

    if(!doc){
        if(areyousure("Pdf error loading", "It seems the pdf file is correct, do you want to remove it?")){
            return load_res_pdf::ok;
        }
        return load_res_pdf::not_valid_pdf;
    }

    cint len = doc->numPages();
    this->resizing_pdf(data, len);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    /* creation of thread and append QImage to the current PDF page */
    for(i = 0; i < len && i < countThread; i++){
        page.append(doc->page(QString::number(i+1)));

    }
#endif

    W_ASSERT(0 < m_image.size());
    auto &list = m_image[0];
    for(i = 0; i < len; i ++){
        list.img.append(imgAppend);
        conv.append(new convertImg(resolution));
    }

    W_ASSERT(len == m_image.at(IndexPdf).img.length());

    for(pdfCount = 0; pdfCount < len; ){
        cint create = qMin(countThread, len);

        W_ASSERT(create <= countThread);

        for(i = 0; i < create && (pdfCount + i) < len; i++){
            QImage *image = &list.img[pdfCount + i];
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            const auto &pdfPage = page.at(pdfCount+i);


            conv.operator[](i)->setData(pdfPage, image);
            conv.at(i)->start();
#else
            conv.operator[](i)->setData(doc, image, pdfCount + i);
            conv.at(i)->start();
#endif
        }

        for(i = 0; i < create && i < conv.length(); i++){
            conv.at(i)->wait();
        }

        pdfCount += countThread;
    }

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    for(i=0; i < page.length(); ++i)
        delete page.at(i);
    delete doc;
#endif

    for(i=0; i < conv.length(); ++i)
        delete conv.at(i);

    for(i = 0; i < m_image.length(); ++i){
        if(this->m_image.at(IndexPdf).img.at(i).isNull()){
            dialog_critic("We had a problem processing an image");
            return load_res_pdf::not_valid_pdf;
        }
    }

    if(FirstLoad)
        this->adjast_pdf(IndexPdf);

    return load_res_pdf::ok;
}

frompdf::load_res_pdf frompdf::save_pdf(const QList<QString>   &path,
                                const QByteArray    &path_writernote_file)
{
    frompdf::load_res_pdf res;

    for(const auto &tmp : qAsConst(path)){

        res = this->save_pdf(tmp.toUtf8(),
                         path_writernote_file);

        if(res != frompdf::load_res_pdf::ok)
            return res;
    }
    return frompdf::load_res_pdf::ok;
}

frompdf::load_res_pdf frompdf::save_pdf(const QByteArray   &pathFile,
                                const QByteArray   &path_writernote_file)
{
    WZipWriter writer;

    if(writer.init(path_writernote_file.constData()) < 0)
        return load_res_pdf::no_valid_path;
    return this->save_pdf(writer, pathFile, path_writernote_file);
}

frompdf::load_res_pdf frompdf::save_pdf(WZipWriter         &filezip,
                                const QByteArray   &pathFile,
                                const QByteArray   &path_writernote_file)
{
    const auto res = savefile::moveFileIntoZip(pathFile,
                                               filezip,
                                               frompdf::getName_pdf(m_image.length()));

    if(unlikely(res != OK))
        return load_res_pdf::not_valid_pdf;

    return load_res_pdf::ok;
}

/*
 * add image from position
*/
void frompdf::addPdf(QByteArray             &pos,
                     const PointSettable    *point,
                     const QByteArray       &path_writernote,
                     datastruct             &data)
{
    int ok;
    frompdf::load_res_pdf res;

    if(path_writernote == "")
        return dialog_critic("Before add a pdf you need to save this file");
    if(m_image.length())
        return user_message("It's not possible to add more than one pdf");

    if(insert_pdf(pos, point) != OK){
        res = load_res_pdf::not_valid_pdf;
        goto err;
    }

    res = this->save_pdf(pos, path_writernote);
    if(res != load_res_pdf::ok)
        goto err;

    res = this->load_pdf(path_writernote, 1, data);

err:
    if(res == load_res_pdf::no_valid_path)
        return dialog_critic(qstr("We had trouble opening %1").arg(path_writernote));
    else if(res != load_res_pdf::ok)
        dialog_critic("We had some error");
}

void frompdf::adjast_pdf(const uchar indexPdf)
{
    m_image.operator[](indexPdf).topLeft = QPointF(0, 0);
}

unsigned frompdf::insert_pdf(QByteArray             &pos,
                             const PointSettable    *point)
{
    assert(this->m_image.length() == 0);
    Pdf pdf;

    if(pos == ""){
        pos = QFileDialog::getOpenFileName(nullptr,
                                                     "Open images",
                                                     "JPEG (*.jpg, *.jpeg);;PNG (*.png);;All Files (*)").toUtf8();

        if(pos == "")
            return ERROR;
    }

    if(point){
        pdf.topLeft = *point;
    }
    else{
        pdf.topLeft = QPointF(0.0, 0.0);
    }

    this->m_image.append(pdf);

    return OK;
}

void frompdf::reset_pdf()
{
    m_image.clear();
}

#endif
