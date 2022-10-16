#include "StrokeProp.h"

#ifdef DEBUGINFO
void StrokeProp::checkType(int type)
{
    W_ASSERT(
            type == COMPLEX_NORMAL or
            type == COMPLEX_CIRCLE or
            type == COMPLEX_LINE or
            type == COMPLEX_RECT
    );
}
#endif

void StrokeProp::setProp(const StrokeProp &prop) noexcept
{
    _prop = prop._prop;
}

#ifdef DEBUFINFO
QDataStream &operator<<(QDataStream &d, const StrokeProp &str)
{
    d << str.toString();
    return d;
}
#endif // DEBUFINFO
