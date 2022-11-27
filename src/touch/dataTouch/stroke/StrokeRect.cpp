#include "StrokeRect.h"

StrokeRect::StrokeRect()
{

}

void StrokeRect::adjust(double zoom)
{
    const QPointF topLeft = _rect.topLeft() / zoom;
    const QPointF bottomRight = _rect.bottomRight() / zoom;
    _rect = QRectF(topLeft, bottomRight);
}

QRect StrokeRect::getBiggerPointInStroke() const
{
    return _rect.toRect();
}
