#pragma once

#ifdef USE_QT
// TODO: raise errer in case ANDROID target is set
# error "Trying use WPainterIos with USE_QT set"
#endif // USE_QT

#include "core/Color/WColor.h"
#include "core/Pen/WPen.h"
#include "core/Image/WImage.h"
#include "core/Pixmap/WPixmap.h"
#include "core/WMutex.h"
#include "core/pointer/UniquePtr.h"

class WPainter
{
protected:
    virtual void execute(const Fn<void()> &function) = 0;

    WPainter() noexcept;
public:
    virtual ~WPainter();

    virtual bool begin(WImage *pixmap) = 0;
    void setColor(const WColor &color);
    void setPen(const WPen &pen);

    void drawLine(const PointF &p1, const PointF &p2);
    void drawLine(int x1, int y1, int x2, int y2);

    void drawImage   (const RectF &target, const WImage &image, const RectF &source);
    void drawPixmap  (const RectF &target, const WPixmap &pixmap, const RectF &source);
    void drawPixmap  (const RectF &target, const WPixmap &pixmap);
    void drawPixmap  (const WRect &target, const WPixmap &pixmap);
    void drawPixmap  (const WRect &target, const WPixmap &pixmap, const WRect &source);
    void drawPoint   (const PointF &point);
    void drawRect    (const RectF &rect);
    void drawEllipse (const PointF &center, double rx, double ry);

    virtual void move (const PointF &point) = 0;
    virtual void addCurve(const PointF &to, const PointF &controll) = 0;
    virtual void closePath () = 0;
    
    enum CompositionMode {
        CompositionMode_Clear,
        CompositionMode_SourceOver,
        CompositionMode_DestinationOver
    };
    void setCompositionMode(enum CompositionMode compositionMode);
    
    [[nodiscard]]
    auto compositionMode() const -> WPainter::CompositionMode;

    void setAntialiasing();
    void setCompositionClear();

    virtual bool end() = 0;
    bool isActive() const;

protected:
    WImage *_target;

    mutable WMutex _lock;
    
    WPen _pen;
    enum CompositionMode _compositionMode;
    bool _isAntialeasing;
    
    int height() const;
    int width() const;
};

inline auto WPainter::isActive() const -> bool
{    
    return _target != nullptr;
}
