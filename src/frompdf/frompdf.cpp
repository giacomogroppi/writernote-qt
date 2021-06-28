#include "frompdf.h"

#include "../utils/dialog_critic/dialog_critic.h"
#include "../currenttitle/currenttitle_class.h"
#include "../utils/permission/permission.h"
#include "../dataread/xmlstruct.h"

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
    FILE *fp;
    QByteArray arr;
    uchar __read;

    fp = fopen(path.toUtf8().constData(), "r");

    if(!fp){
        return load_res::not_valid_pdf;
    }

    do {
        fread(&__read, 1, 1, fp);
        arr.append(__read);
    }
    while(!feof(fp));


    return load_from_row(arr, clear);
}

frompdf::load_res frompdf::load(zip_t *fileZip, const bool clear)
{
    zip_file_t *fp;
    QByteArray arr;
    uchar __read;
    size_t size;

    uint i;

    for(i=0; i<m_data->count_pdf; ++i){
        const QString &ref_str = frompdf::getName(m_data->nome_copybook, i);

        size = xmlstruct::sizeFile(fileZip, ref_str);
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
        }


        load_from_row(arr, clear);
    }

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
