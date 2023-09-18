#pragma once

#ifdef USE_QT

#include "touch/dataTouch/Point.h"

#include <QPen>
#include <QBrush>

class WPen {
private:
    QPen _pen;
public:
    WPen();

    void setBrush(const QBrush &brush);

    void setColorNull();
    void setColor(const WColor &color);
    WColor color() const;
    double widthF() const;

    void setWidthF(double value);

    enum WPenStyle {
        DotLine
    };
    void setStyle(enum WPenStyle style);

    friend class WPainter;
};

#endif // USE_QT
