#include "WPenIos.h"

#if is_ios || (is_mac && !USE_QT)
WPen::WPen()
    : _color(color_black)
    , _size(1.)
    , _style(WPenStyle::DotLine)
{
    
}

auto WPen::setColorNull() -> WPen&
{
    this->_color = color_transparent;
    
    return *this;
}

auto WPen::setColor(const WColor &color) -> WPen&
{
    this->_color = color;
    return *this;
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

auto WPen::setWidthF(double value) -> WPen&
{
    this->_size = value;
    return *this;
}

auto WPen::setStyle(enum WPenStyle style) -> WPen&
{
    this->_style = style;
    return *this;
}
#endif
