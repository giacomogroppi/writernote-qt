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

bool StrokeLine::isEmpty() const
{
    return false;
}
