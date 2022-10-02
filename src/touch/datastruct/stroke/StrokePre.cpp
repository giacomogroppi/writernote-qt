#include "StrokePre.h"

StrokePre::StrokePre() noexcept :
    Stroke(),
    WImage()
{
    WImage::initAsPage();
}

void StrokePre::merge(Stroke &to) const noexcept
{
    W_ASSERT(to.isEmpty() == true);
}

void StrokePre::reset() noexcept
{
    Stroke::reset();
    WImage::initAsPage();
}
