#pragma once

#include "touch/dataTouch/Point.h"
#include "utils/platform.h"
#include "core/Color/WColor.h"

#if is_ios || (is_mac && !USE_QT)
class WPen
{
public:
    WPen();
    ~WPen() = default;

    auto setColorNull() -> WPen&;
    auto setColor(const WColor &color) -> WPen&;
    WColor color() const;
    double widthF() const;

    // todo --> try to remove
    auto setSolidPattern() -> WPen&;

    auto setWidthF(double value) -> WPen&;

    enum WPenStyle {
        DotLine
    };
    auto setStyle(enum WPenStyle style) -> WPen&;
    
    friend class WPainter;
private:
    WColor _color;
    double _size;
    WPenStyle _style;
};
#endif
