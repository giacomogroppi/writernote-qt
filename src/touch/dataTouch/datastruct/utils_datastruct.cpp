#include "utils_datastruct.h"
#include "touch/dataTouch/stroke/Stroke.h"

bool datastruct_isinside(const PointF &topleft, const PointF &bottonright, const Stroke &stroke)
{
    return stroke.isInside(RectF(topleft, bottonright));
}
