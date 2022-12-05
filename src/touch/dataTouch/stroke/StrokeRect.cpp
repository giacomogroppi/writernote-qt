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

int StrokeRect::how_much_decrese() const
{
    return 0;
}

QRect StrokeRect::getBiggerPointInStroke() const
{
    return _rect.toRect();
}
