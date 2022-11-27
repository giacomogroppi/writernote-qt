#include "StrokeCircle.h"

StrokeCircle::StrokeCircle()
{

}

void StrokeCircle::adjust(double zoom)
{
    _r /= zoom;
    _x /= zoom;
    _y /= zoom;
}

QRect StrokeCircle::getBiggerPointInStroke() const
{
    const auto topLeft = QPoint(_x - _r, _y - _r);
    const auto bottomRight = QPoint(_x + _r, _y + _r);
    return QRect(topLeft, bottomRight);
}
