#include "convertImg.h"
#include <QDebug>
#include "utils/common_script.h"

#ifdef PDFSUPPORT

void convertImg::run()
{
    W_ASSERT(img);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    *img = page->renderToImage(precision, precision);
#else
    *img = doc->page(index)->renderToImage(precision, precision);
#endif
}

convertImg::convertImg(const uint precision) :
    QThread(nullptr)
{
    this->precision = precision;
}

convertImg::~convertImg()
{

}

#endif
