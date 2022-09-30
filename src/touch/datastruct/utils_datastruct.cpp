#include "utils_datastruct.h"
#include "touch/datastruct/stroke/Stroke.h"

bool datastruct_isinside(const QPointF &topleft, const QPointF &bottonright, const Stroke &stroke)
{
    return stroke.isInside(QRectF(topleft, bottonright));
}
