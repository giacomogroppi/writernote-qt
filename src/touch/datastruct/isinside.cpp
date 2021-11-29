#include "datastruct.h"
#include <QDebug>

bool datastruct::isinside(const QPointF &topleft, const QPointF &bottonright, const stroke &stroke)
{
    return stroke.isInside(QRectF(topleft, bottonright));
}

bool datastruct::isinside(const QPointF &topleft, const QPointF &bottomright, const uint IndexPage, const uint IndexStroke) const
{
    const stroke &stroke = at(IndexPage).atStroke(IndexStroke);
    return datastruct::isinside(topleft, bottomright, stroke);
}
