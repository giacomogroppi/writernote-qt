#include "StrokeRect.h"
#include <QPainter>
#include "touch/dataTouch/page/Page.h"

StrokeRect::StrokeRect()
{

}

void StrokeRect::draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const
{
    painter.setPen(pen);

    set_press(pen, _data.press, prop, is_rubber, this->getColor(1.));

    const auto _topLeft     = Page::at_translation(
                point_s(
                    _data.rect.topLeft()),
                    page
                ).toQPointF(prop);

    const auto _bottomRight = Page::at_translation(
                point_s(
                    _data.rect.bottomRight()),
                    page
                ).toQPointF(prop);

    painter.drawRect(QRectF(_topLeft, _bottomRight));
}

void StrokeRect::adjust(double zoom)
{
    const QPointF topLeft = _data.rect.topLeft() / zoom;
    const QPointF bottomRight = _data.rect.bottomRight() / zoom;
    _data.rect = QRectF(topLeft, bottomRight);
}

int StrokeRect::how_much_decrese() const
{
    return 0;
}

QRect StrokeRect::getBiggerPointInStroke() const
{
    return _data.rect.toRect();
}
