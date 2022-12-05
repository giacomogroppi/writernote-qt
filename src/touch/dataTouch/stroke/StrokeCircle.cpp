#include "StrokeCircle.h"
#include "touch/dataTouch/page/Page.h"

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

void StrokeCircle::draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop_) const
{
    constexpr bool not_used debCircle = false;
    point_s point;
    QPointF tmp;
    const auto press = _data.press;
    double y, x;
    cdouble prop = prop_ == PROP_RESOLUTION ? prop_ : 1.;

    point = point_s(_data.x, _data.y);

    point = Page::at_translation(point, page);
    tmp = point.toQPointF(prop);

    WDebug(debCircle, "prop: " << prop);

    y = tmp.y();
    x = tmp.x();

    set_press(pen, press, prop, is_rubber, this->getColor(1.));
    painter.setPen(pen);

    painter.drawEllipse(QPointF(x, y), _data.r * prop, _data.r * prop);

    WDebug(debCircle, _data.x << _data.y << _data.r);
}

int StrokeCircle::is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const
{
    QPointF tl, br;

    constexpr bool not_used debug = true;

    W_ASSERT(_data.x >= 0.);
    W_ASSERT(_data.r >= 0.);
    W_ASSERT(_data.y >= 0.);
    W_ASSERT(_data.press >= 0.);

    line.get_point(tl, br);

    cdouble distance1 = distanceFromCenter(tl);
    cdouble distance2 = distanceFromCenter(br);

    WDebug(debug, distance1 << distance2 << tl << br);

    if(     WCommonScript::is_near(distance1, _data.r, precision) or
            WCommonScript::is_near(distance2, _data.r, precision))
        return true;

    cbool res = oneSide(distance1, distance2, precision) ||
                oneSide(distance2, distance1, precision);

    return res;
}

void StrokeCircle::append(const point_s &point, pressure_t pressure)
{
    Q_UNUSED(pressure);
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
