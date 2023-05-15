#pragma once

#if defined(USE_QT)
# include <QPainter>
#endif // USE_QT
#include "core/WImage.h"
#include "touch/dataTouch/Point.h"
#include "core/Rect.h"
#include "core/WPixmap.h"
#include "core/RectF.h"
#include "core/WPen.h"

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
    WPainter();
    ~WPainter();

    bool begin(WPixmap *pixmap);
    void begin(WImage *image);
    void setColor(const colore_s &color);
    void setPen(WPen &pen);

    void drawLine(const PointF &p1, const PointF &p2);
    void drawImage(const WImage &img, const Rect &source, const Rect &target);
    void drawPixmap(const RectF &target, const WPixmap &pixmap, const RectF &source);
    void drawPoint(const PointF &point);

    void fillRect(const RectF &rect);

    void setAntialeasing();
    void setCompositionClear();

    bool end();
};

inline void WPainter::setColor(const colore_s &color)
{
    this->_color = color;
}

