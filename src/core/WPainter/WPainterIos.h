#pragma once

#ifdef USE_QT
// TODO: raise errer in case ANDROID target is set
# error "Trying use WPainterIos with USE_QT set"
#endif // USE_QT

#include "core/Pen/WPen.h"
#include "core/Image/WImage.h"
#include "core/Pixmap/WPixmap.h"

class WPainter
{
private:
public:
    WPainter();
    ~WPainter();

    bool begin(WPixmap *pixmap);
    void setColor(const WColor &color);
    void setPen(WPen &pen);

    void drawLine(const PointF &p1, const PointF &p2);
    void drawLine(int x1, int y1, int x2, int y2);

    void drawImage(const RectF &target, const WImage &image, const RectF &source);
    void drawPixmap(const RectF &target, const WPixmap &pixmap, const RectF &source);
    void drawPixmap(const RectF &target, const WPixmap &pixmap);
    void drawPixmap(const WRect &target, const WPixmap &pixmap);
    void drawPixmap(const WRect &target, const WPixmap &pixmap, const WRect &source);
    void drawPoint(const PointF &point);
    void drawRect(const RectF &rect);
    void drawEllipse(const PointF &center, double rx, double ry);

    enum CompositionMode {
        CompositionMode_Clear,
        CompositionMode_SourceOver,
        CompositionMode_DestinationOver
    };
    void setCompositionMode(enum CompositionMode compositionMode);
    [[nodiscard]] WPainter::CompositionMode compositionMode() const;

    void setAntialeasing();
    void setCompositionClear();


    bool end();
    bool isActive() const;
};
