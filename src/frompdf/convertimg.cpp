#include "convertImg.h"
#include <QDebug>
#include "utils/common_script.h"

#ifdef PDFSUPPORT

void convertImg::run()
{
    W_ASSERT(page);
    W_ASSERT(img);
    *img = page->renderToImage(precision, precision);
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
