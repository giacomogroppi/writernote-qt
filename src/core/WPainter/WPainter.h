#pragma once

#ifdef USE_QT
# include <QPainter>
# include <QBrush>
# include "QtGui/qpdfwriter.h"
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
    QPainter *_painter;
    bool _allocated;
#elif IOS
#elif ANDROID

#endif

#ifndef USE_QT
    WImage &_img;
    WColor _color;
#endif

public:
    WPainter();
    ~WPainter();

#ifdef USE_QT
    WPainter (QPainter *painter);
    WPainter (QPdfWriter *pdfWriter);
    void setBrush(const QBrush &brush);
#endif // USE_QT

    bool begin(WPixmap *pixmap);
    void begin(WImage *image);
    void setColor(const WColor &color);
    void setPen(WPen &pen);

    void drawLine(const PointF &p1, const PointF &p2);
    void drawImage(const RectF &target, const WImage &image, const RectF &source);
    void drawPixmap(const RectF &target, const WPixmap &pixmap, const RectF &source);
    void drawPixmap(const RectF &target, const WPixmap &pixmap);
    void drawPixmap(const Rect &target, const WPixmap &pixmap);
    void drawPixmap(const Rect &target, const WPixmap &pixmap, const Rect &source);
    void drawPoint(const PointF &point);
    void drawRect(const RectF &rect);
    void drawEllipse(const PointF &center, double rx, double ry);

    enum CompositionMode {
        CompositionMode_Clear,
        CompositionMode_SourceOver,
        CompositionMode_DestinationOver
    };
    void setCompositionMode(enum CompositionMode compositionMode);
    WPainter::CompositionMode compositionMode() const;

    void fillRect(const RectF &rect);

    void setAntialeasing();
    void setCompositionClear();


    bool end();
    bool isActive() const;
};

#ifdef USE_QT
inline WPainter::WPainter(QPainter *painter)
    : _painter(painter)
    , _allocated(false)
{
}
#endif // USE_QT


inline void WPainter::setColor(const WColor &color)
{
#ifdef USE_QT
    this->_painter->setPen(color.toQColor());
#else
    this->_color = color;
#endif
}

