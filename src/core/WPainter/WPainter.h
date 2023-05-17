#pragma once

#if defined(USE_QT)
# include <WPainter>
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
    WPainter _painter;
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
    void drawImage(const RectF &target, const WImage &image, const RectF &source);
    void drawPixmap(const RectF &target, const WPixmap &pixmap, const RectF &source);
    void drawPixmap(const RectF &target, const WPixmap &pixmap);
    void drawPoint(const PointF &point);
    void drawRect(const RectF &rect);
    void drawEllipse(const PointF &center, double rx, double ry);

    enum CompositionMode {
        CompositionMode_Clear,
        CompositionMode_SourceOver
    };
    void setCompositionMode(enum CompositionMode compositionMode);
    WPainter::CompositionMode compositionMode() const;

    void fillRect(const RectF &rect);

    void setAntialeasing();
    void setCompositionClear();

    bool end();
    bool isActive() const;
};

inline void WPainter::setColor(const colore_s &color)
{
    this->_color = color;
}

