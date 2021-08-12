#include "frompdf.h"

#include "../utils/dialog_critic/dialog_critic.h"
#include "../currenttitle/document.h"
#include "../utils/permission/permission.h"
#include "../dataread/xmlstruct.h"
#include "../dataread/load_from_file.h"
#include "../dataread/readlistarray.h"
#include "../datawrite/savefile.h"

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

frompdf::load_res frompdf::load(zip_t *fileZip, const bool clear)
{
    QList<QByteArray> arr;
    QStringList __name;
    uint i;

    __name = get_name_pdf();

    if(readListArray::read(__name, fileZip, arr, clear) != OK){
        return frompdf::load_res::not_valid_pdf;
    }

    for (i=0; i<m_data->count_pdf; ++i){
        auto res = load_from_row(arr.at(i), false);
        if(res != frompdf::load_res::ok)
            return res;
    }

    return frompdf::load_res::ok;

}

frompdf::load_res frompdf::load_from_row(const QByteArray &pos, const bool clear)
{
    uint i, len;
    QImage image;

    if(clear)
        this->reset();

    doc = Poppler::Document::loadFromData(pos);

    if(!doc){
        dialog_critic("The file is not readable");
        return load_res::not_valid_pdf;
    }

    len = doc->numPages();

    for(i=0; i<len; ++i){
        image = doc->page(QString::number(i))->renderToImage(
                                    5 * IMG_PDF_HEIGHT,
                                    5 * IMG_PDF_HEIGHT);

        if(image.isNull())
            return load_res::not_valid_page;

        m_image.append(image);

    }


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
    }
}

frompdf::load_res frompdf::save(zip_t *filezip,
                                const QString &path,
                                const QString &path_writernote_file)
{
    if(savefile::saveArrayIntoFile(path,
                                   this->m_data->nome_copybook,
                                   path_writernote_file,
                                   filezip,
                                   frompdf::getNameNoCopy(m_data->count_pdf)) != OK)
        return load_res::not_valid_pdf;
    return load_res::ok;
}
