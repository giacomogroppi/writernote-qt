#include "WPenIos.h"

WPen::WPen()
    : _style(WPenStyle::DotLine)
    , _size(1.)
    , _color(color_black)
{
    
}

auto WPen::setColorNull() -> void
{
    this->_color = color_transparent;
}

void WPen::setColor(const WColor &color)
{
    this->_color = color;
}

auto WPen::color() const -> WColor
{
    return this->_color;
}

auto WPen::widthF() const -> double
{
    return this->_size;
}

void WPen::setWidthF(double value)
{
    this->_size = value;
}

void WPen::setStyle(enum WPenStyle style)
{
    this->_style = style;
}
