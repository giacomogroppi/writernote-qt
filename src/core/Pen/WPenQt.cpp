#include "touch/dataTouch/page/Page.h"

#ifdef USE_QT

#include "WPenQt.h"

WPen::WPen()
{
    this->_pen = QPen(QBrush(), 1.0, Qt::SolidLine, Qt::MPenCapStyle, Qt::RoundJoin);
}

void WPen::setBrush(const QBrush &brush)
{
    this->_pen.setBrush(brush);
}

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

auto WPen::widthF() const -> double
{
    return this->_pen.widthF();
}

auto WPen::color() const -> WColor
{
    return {this->_pen.color()};
}

#endif // USE_QT
