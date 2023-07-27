#pragma once

#include "touch/dataTouch/Point.h"

class WPen
{
public:
    WPen();
    ~WPen() = default;

    void setColorNull();
    void setColor(const WColor &color);
    WColor color() const;
    double widthF() const;

    // todo --> try to remove
    void setSolidPattern();

    void setWidthF(double value);

    enum WPenStyle {
        DotLine
    };
    void setStyle(enum WPenStyle style);

    friend class WPainter;
private:
    WColor _color;
    double _size;
    WPenStyle _style;
};
