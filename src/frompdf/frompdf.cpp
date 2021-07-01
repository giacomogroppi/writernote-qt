#include "frompdf.h"

#include "../utils/dialog_critic/dialog_critic.h"
#include "../currenttitle/currenttitle_class.h"
#include "../utils/permission/permission.h"
#include "../dataread/xmlstruct.h"
#include "../dataread/load_from_file.h"

frompdf::frompdf(currenttitle_class *data)
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
    /*zip_file_t *fp;
    uchar __read;
    size_t size;*/

    QByteArray arr;

    QStringList __name;
    uint i;

    if(clear)
        arr.clear();

    __name = get_name_pdf();


    for(i=0; i<m_data->count_pdf; ++i){
        const QString &ref_str = __name.at(i);

        xmlstruct::readFile(fileZip, arr, clear, ref_str, false);

        /*size = xmlstruct::sizeFile(fileZip, ref_str);
        fp = zip_fopen(fileZip,
                       ref_str.toUtf8().constData()
                       , 0);


        if(!fp){
            return load_res::not_valid_pdf;
        }

        while(size) {
            zip_fread(fp, &__read, 1);
            arr.append(__read);
            --size;
        }*/

        load_from_row(arr, false);
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
