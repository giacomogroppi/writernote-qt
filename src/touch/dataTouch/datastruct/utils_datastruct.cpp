#include "utils_datastruct.h"
#include "touch/dataTouch/stroke/Stroke.h"

bool datastruct_isinside(const QPointF &topleft, const QPointF &bottonright, const Stroke &stroke)
{
    return stroke.isInside(QRectF(topleft, bottonright));
}
