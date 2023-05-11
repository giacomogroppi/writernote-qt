#pragma once

#if defined(USE_QT)
# include <QPainter>
#endif // USE_QT
#include "core/WImage.h"
#include "touch/dataTouch/Point.h"
#include "core/Rect.h"

class WPainter
{
private:
#ifdef USE_QT
    QPainter _painter;
#elif IOS
#elif ANDROID

#endif

#ifndef USE_QT
    WImage &_img;
    colore_s _color;
#endif

public:
#ifdef USE_QT
    explicit WPainter(QPaintDevice *img);
#else
    explicit WPainter(WImage &img);
#endif

    void setColor(const colore_s &color);

    void drawLine(const PointF &p1, const PointF &p2);
    void drawImage(const WImage &img, const Rect &source, const Rect &target);

    bool end();
};

inline void WPainter::setColor(const colore_s &color)
{
    this->_color = color;
}

