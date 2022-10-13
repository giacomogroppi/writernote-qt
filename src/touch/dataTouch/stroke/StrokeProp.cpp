#include "StrokeProp.h"

void StrokeProp::checkType(int type)
{
    W_ASSERT(
            type == COMPLEX_NORMAL or
            type == COMPLEX_CIRCLE or
            type == COMPLEX_LINE or
            type == COMPLEX_RECT
    );
}

inline void StrokeProp::set(const StrokeProp &prop) noexcept
{
    _prop = prop._prop;
}
