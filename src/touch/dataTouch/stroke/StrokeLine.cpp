#include "StrokeLine.h"
#include "touch/dataTouch/page/Page.h"

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

void StrokeLine::adjust(double zoom)
{
    this->_pt1 /= zoom;
    this->_pt2 /= zoom;
    this->_press /= zoom;
}
