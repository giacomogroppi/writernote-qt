#include "StrokeLine.h"
#include "touch/dataTouch/page/Page.h"
#include "core/WLine.h"

StrokeLine::StrokeLine()
{

}

void StrokeLine::draw(QPainter &painter,
                      cbool is_rubber,
                      cint page,
                      QPen &pen,
                      cdouble prop) const
{
    const auto p = prop == PROP_RESOLUTION ? prop : 1.;

    painter.setPen(pen);
    set_press(pen, this->_press, p, is_rubber, this->getColor(1.));

    const auto _topLeft     = Page::at_translation(point_s(this->_pt1), page).toQPointF(prop);
    const auto _bottomRight = Page::at_translation(point_s(this->_pt2), page).toQPointF(prop);

    painter.setPen(pen);
    painter.drawLine(_topLeft, _bottomRight);
}

int StrokeLine::is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const
{
    Q_UNUSED(needToDeletePoint);
    W_ASSERT(from == 0);
    WLine _line(this->_pt1, this->_pt2);
    return WLine::intersect(_line, line, precision);
}

void StrokeLine::append(const point_s &point, pressure_t pressure)
{
    using namespace WCommonScript;
    const auto dist1 = distance_not_square(_pt1, point);
    const auto dist2 = distance_not_square(_pt2, point);

    Q_UNUSED(pressure);

    if(dist1 > dist2){
        _pt2 = point;
    }else{
        _pt1 = point;
    }
}

void StrokeLine::decreasePrecision()
{
}

void StrokeLine::adjust(double zoom)
{
    this->_pt1 /= zoom;
    this->_pt2 /= zoom;
    this->_press /= zoom;
}

void StrokeLine::makeNormalVertical(StrokeNormal *mergeTo, int from, int to) const
{
    point_s tmp;
    W_ASSERT(_pt2.x() == _pt1.x());
    W_ASSERT(from <= to);

    tmp.rx() = _pt1.x();

    for(; from <= to; from ++){
        tmp.ry() = from;
        mergeTo->append(tmp, _press);
    }
}

void StrokeLine::makeNormalGeneric(StrokeNormal *mergeTo, int from, int to) const
{
    double m, p;
    point_s point;

    W_ASSERT(from <= to);

    m =     (_pt1.y() - _pt2.y()) /
            (_pt1.x() - _pt2.x());

    p = _pt1.y() - _pt1.x() * m;

    for(; from <= to; from ++){
        const double x = (double(from) - p) / m;
        point.rx() = x;
        point.ry() = (double) from;
        mergeTo->append(point, _press);
    }
}

Stroke *StrokeLine::makeNormal() const
{
    int from, to;
    auto *res = new StrokeNormal();

    W_ASSERT(res->isEmpty());

    from    = (int) _pt1.y();
    to      = (int) _pt2.y();

    W_ASSERT(from <= to);

    if(_pt2.x() == _pt1.x()){
        this->makeNormalVertical(res, from, to);
    }else{
        this->makeNormalGeneric(res, from, to);
    }

    return res;
}

bool StrokeLine::isEmpty() const
{
    return false;
}

QRect StrokeLine::getBiggerPointInStroke() const
{
    return datastruct_rect(_pt1, _pt2).toRect();
}

bool StrokeLine::operator==(const Stroke &other)
{
    if(dynamic_cast<Stroke &>(*this) != other)
        return false;
    if(this->type() != other.type())
        return false;

    const auto *s = dynamic_cast<const StrokeLine *>(&other);

    return  this->_pt1 == s->_pt1 and
            this->_pt2 == s->_pt2 and
            this->_press == s->_press;
}
