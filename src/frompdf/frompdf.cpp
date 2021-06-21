#include "frompdf.h"

#include "../utils/dialog_critic/dialog_critic.h"
#include "../currenttitle/currenttitle_class.h"
#include "../utils/permission/permission.h"

frompdf::frompdf(currenttitle_class *data)
{
    m_data = data;
}

#define IMG_PDF_HEIGHT 292
#define IMG_PDF_WIDTH 210
frompdf::load_res frompdf::load(const QString &pos)
{
    uint i, len;
    QImage image;

    images.clear();
    doc = Poppler::Document::load(pos);

    if(!doc){
        dialog_critic("The file is not readable");
        return load_res::not_valid_pdf;
    }

    len = doc->numPages();

    for(i=0; i<len; ++i){
        image = doc->page("0")->renderToImage(
                                    5 * IMG_PDF_HEIGHT,
                                    5 * IMG_PDF_HEIGHT);

        if(image.isNull())
            return load_res::not_valid_page;

        images.append(image);

    }


    return load_res::ok;
}
