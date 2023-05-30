//
// Created by Giacomo Groppi on 08/05/23.
//

#include "WPen.h"

WPen::WPen()
{
#ifdef USE_QT
    this->_pen = QPen(QBrush(), 1.0, Qt::SolidLine, Qt::MPenCapStyle, Qt::RoundJoin);
#else
#endif
}

#ifdef USE_QT
void WPen::setBrush(const QBrush &brush)
{
    this->_pen.setBrush(brush);
}

#include "touch/dataTouch/page/Page.h"
void WPen::setColorNull()
{
    this->setColor(COLOR_NULL);
}

void WPen::setColor(const WColor &color)
{
    this->_pen.setColor(color.toQColor());
}

void WPen::setWidthF(double value)
{
    this->_pen.setWidthF(value);
}

void WPen::setStyle(WPenStyle style)
{
    Qt::PenStyle s;
    switch (style) {
    case WPenStyle::DotLine:
        s = Qt::PenStyle::DotLine;
        break;
    default:
        break;
    }

    this->_pen.setStyle(s);
}

double WPen::widthF() const {
    return this->_pen.widthF();
}

WColor WPen::color() const
{
    return {this->_pen.color()};
}

#endif // USE_QT
