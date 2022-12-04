#include "StrokeCircle.h"

int StrokeCircle::load(WZipReaderSingle &reader)
{
    struct {
        double _x, _y, _r;
        pressure_t _press;
    };
}

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
