#include "StrokeCircle.h"

int StrokeCircle::load(WZipReaderSingle &reader)
{
    W_ASSERT(0);
}

StrokeCircle::StrokeCircle()
{

}

double StrokeCircle::distanceFromCenter(const QPointF &point) const
{
    using namespace WCommonScript;
    return qSqrt(
        Power(_data.x - point.x(), 2) +
        Power(_data.y - point.y(), 2)
                );
}

int StrokeCircle::isInternal(double distance, double precision) const
{
    const auto raggio = qSqrt(_data.r);
    const auto real_dist1 = qSqrt(distance);
    return real_dist1 <= raggio + precision;
}

bool StrokeCircle::oneSide(double inside, double outside, double prec) const
{
    return  isInternal(inside, prec) and
            not isInternal(outside, prec);
}

void StrokeCircle::append(const point_s &point, pressure_t pressure)
{
    _data.r = WCommonScript::distance(QPointF(_data.x, _data.y), point);
}

void StrokeCircle::adjust(double zoom)
{
    _data.r /= zoom;
    _data.x /= zoom;
    _data.y /= zoom;
}

QRect StrokeCircle::getBiggerPointInStroke() const
{
    const auto topLeft = QPoint(_data.x - _data.r, _data.y - _data.r);
    const auto bottomRight = QPoint(_data.x + _data.r, _data.y + _data.r);
    return QRect(topLeft, bottomRight);
}
