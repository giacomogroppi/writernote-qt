#include "datastruct.h"
#include <QDebug>

bool datastruct::isinside(const QPointF &topleft, const QPointF &bottonright, const stroke &stroke)
{
    return stroke.isInside(QRectF(topleft, bottonright));
}

bool datastruct::isinside(const QPointF &topleft, const QPointF &bottonright, const QPointF &point)
{
    return  topleft.x() <= point.x()
            && topleft.y() <= point.y()
            && bottonright.x() >= point.x()
            && bottonright.y() >= point.y();
}

bool datastruct::isinside(const QPointF &topleft, const QPointF &bottomright, const uint IndexPage, const uint IndexStroke) const
{
    const stroke &stroke = at(IndexPage).atStroke(IndexStroke);
    return datastruct::isinside(topleft, bottomright, stroke);
}
