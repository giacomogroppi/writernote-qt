#include "frompdf.h"

#include "../utils/dialog_critic/dialog_critic.h"
#include "../currenttitle/currenttitle_class.h"
#include "../utils/permission/permission.h"

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

frompdf::load_res frompdf::load(const QString &pos, const bool clear)
{
    uint i, len;
    QImage image;

    if(clear)
        this->reset();

    doc = Poppler::Document::load(pos);

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
