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
public:
    WPainter() noexcept;
    ~WPainter() = default;

    bool begin(WImage *pixmap);
    void setColor(const WColor &color);
    void setPen(const WPen &pen);

    void drawLine(const PointF &p1, const PointF &p2); // done
    void drawLine(int x1, int y1, int x2, int y2);

    void drawImage   (const RectF &target, const WImage &image, const RectF &source); // done
    void drawPixmap  (const RectF &target, const WPixmap &pixmap, const RectF &source); // done
    void drawPixmap  (const RectF &target, const WPixmap &pixmap); // done
    void drawPixmap  (const WRect &target, const WPixmap &pixmap); // done
    void drawPixmap  (const WRect &target, const WPixmap &pixmap, const WRect &source); // done
    void drawPoint   (const PointF &point);
    void drawRect    (const RectF &rect);
    void drawEllipse (const PointF &center, double rx, double ry);

    enum CompositionMode {
        CompositionMode_Clear,
        CompositionMode_SourceOver,
        CompositionMode_DestinationOver
    };
    void setCompositionMode(enum CompositionMode compositionMode);
    
    [[nodiscard]]
    auto compositionMode() const -> WPainter::CompositionMode;

    void setAntialeasing();
    void setCompositionClear();

    bool end();
    bool isActive() const;
    
private:
    WImage *_target;
    WPen _pen;
    WColor _color;
    enum CompositionMode _compositionMode;
    bool _isAntialeasing;
};

inline void WPainter::drawPixmap(const RectF &target, const WPixmap& pixmap, const RectF &source)
{
    const WImage &image = static_cast<const WImage&>(pixmap);
    return this->drawImage(target, image, source);
}

inline void WPainter::drawPixmap  (const RectF &target, const WPixmap &pixmap)
{
    const RectF source = pixmap.rect();
    return this->drawPixmap(target, pixmap, source);
}

inline void WPainter::drawPixmap  (const WRect &target, const WPixmap &pixmap)
{
    const RectF source = pixmap.rect().castTo<double>();
    const RectF targetCasted = target.castTo<double>();
    return this->drawPixmap(targetCasted, pixmap, source);
}

inline void WPainter::drawPixmap  (const WRect &target, const WPixmap &pixmap, const WRect &source)
{
    const RectF sourceCasted = source.castTo<double>();
    const RectF targetCasted = target.castTo<double>();
    return this->drawPixmap(targetCasted, pixmap, sourceCasted);
}

inline void WPainter::setColor(const WColor &color)
{
    this->_color = color;
}

inline void WPainter::setPen(const WPen &pen)
{
    this->_pen = pen;
}

inline void WPainter::setAntialeasing()
{
    this->_isAntialeasing = true;
}

inline auto WPainter::compositionMode() const -> WPainter::CompositionMode
{
    return this->_compositionMode;
}

inline void WPainter::drawLine (const PointF &p1, const PointF &p2)
{
    return this->drawLine (p1.x(), p1.y(), p2.x(), p2.y());
}

inline void WPainter::setCompositionMode(enum CompositionMode compositionMode)
{
    this->_compositionMode = compositionMode;
}
