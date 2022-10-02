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

Stroke& StrokePre::merge()
{
    W_ASSERT(this->already_merge == false);

    WList<point_s>::ConstIterator it1 = _point.constBegin();
    WList<point_s>::ConstIterator it2 = _pressure.constBegin();

    Stroke &res = *this;

    for(; it1 ++, it2 ++){

    }

#ifdef DEBUGINFO
    already_merge = true;
#endif // DEBUGINFO
}
