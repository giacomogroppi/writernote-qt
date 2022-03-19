#include "utils/common_script.h"
#include "core/wline.h"


#ifdef DEBUGINFO

void __init__ testingWLine(void)
{
    WLine line1(QPointF(100., 100.), QPointF(100., 200.));
    WLine line2(QPointF(95., 90.), QPointF(101., 175.));
    WLine line3(QPointF(95., 110.), QPointF(101., 110.));
    W_ASSERT(line1.intersect(line2, 0.));
    W_ASSERT(line1.intersect(line2, 0.));
}

#endif //DEBUGINFO
