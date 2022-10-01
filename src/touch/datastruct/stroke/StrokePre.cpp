#include "StrokePre.h"

StrokePre::StrokePre() :
    Stroke(),
    WImage()
{
    WImage::initAsPage();
}

void StrokePre::reset()
{
    Stroke::reset();
    WImage::initAsPage();
}
