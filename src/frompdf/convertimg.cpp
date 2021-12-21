#include "convertImg.h"
#include <QDebug>
#ifdef PDFSUPPORT

void convertImg::run(){
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
