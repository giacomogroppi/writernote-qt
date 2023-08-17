#include "WPenIos.h"

#if is_ios || (is_mac && !USE_QT)
WPen::WPen()
    : _color(color_black)
    , _size(1.)
    , _style(WPenStyle::DotLine)
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

// TODO: change the name to getColor
auto WPen::color() const -> WColor
{
    return this->_color;
}

// TODO: change the name to getWidthF
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
#endif
