#pragma once

#ifdef USE_QT
// TODO: raise errer in case ANDROID target is set
# error "Trying use WPainterIos with USE_QT set"
#endif // USE_QT

#include "core/Pen/WPen.h"
#include "core/Image/WImage.h"
#include "core/Pixmap/WPixmap.h"
#include "core/WMutex.h"
#include "core/pointer/UniquePtr.h"

class WPainter
{
public:
    WPainter() noexcept; // done
    ~WPainter(); // done

    bool begin(WImage *pixmap); // done
    void setColor(const WColor &color); // done
    void setPen(const WPen &pen); // done

    void drawLine(const PointF &p1, const PointF &p2); // done
    void drawLine(int x1, int y1, int x2, int y2); // done

    void drawImage   (const RectF &target, const WImage &image, const RectF &source); // done
    void drawPixmap  (const RectF &target, const WPixmap &pixmap, const RectF &source); // done
    void drawPixmap  (const RectF &target, const WPixmap &pixmap); // done
    void drawPixmap  (const WRect &target, const WPixmap &pixmap); // done
    void drawPixmap  (const WRect &target, const WPixmap &pixmap, const WRect &source); // done
    void drawPoint   (const PointF &point);
    void drawRect    (const RectF &rect);
    void drawEllipse (const PointF &center, double rx, double ry); // done

    enum CompositionMode {
        CompositionMode_Clear,
        CompositionMode_SourceOver,
        CompositionMode_DestinationOver
    };
    void setCompositionMode(enum CompositionMode compositionMode); // done
    
    [[nodiscard]]
    auto compositionMode() const -> WPainter::CompositionMode; // done

    void setAntialeasing(); // done
    void setCompositionClear(); // done

    bool end();
    bool isActive() const;
    
private:
    mutable WMutex _lock;
    WImage *_target;
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
