#include "utils_datastruct.h"
#include "src/touch/datastruct/stroke.h"

bool datastruct_isinside(const QPointF &topleft, const QPointF &bottonright, const stroke &stroke)
{
    return stroke.isInside(QRectF(topleft, bottonright));
}