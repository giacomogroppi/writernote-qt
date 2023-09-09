#pragma once

#ifndef USE_QT
# error "Trying use WPainter Qt version without USE_QT set"
#endif // USE_QT

#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include "QtGui/qpdfwriter.h"

#include "core/Image/WImage.h"
#include "touch/dataTouch/Point.h"
#include "core/Rect.h"
#include "core/Pixmap/WPixmap.h"
#include "core/RectF.h"
#include "core/Pen/WPen.h"

class WPainter
{
private:
    QPainter *_painter;
    bool _allocated;

public:
    WPainter();
    ~WPainter();

    explicit WPainter (QPainter *painter);
    WPainter (QPdfWriter *pdfWriter);
    void setBrush(const QBrush &brush);

    bool begin(WPixmap *pixmap);
    void begin(WImage *image);
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

    void setAntialiasing();
    void setCompositionClear();

    void move (const PointF& point) noexcept;
    void addCurve(const PointF &to, const PointF &controll) noexcept;
    void closePath () noexcept;

    bool end();
    bool isActive() const;

private:
    QPainterPath _path;
};

inline WPainter::WPainter(QPainter *painter)
    : _painter(painter)
    , _allocated(false)
{
}

inline void WPainter::setColor(const WColor &color)
{
    this->_painter->setPen(color.toQColor());
}

inline WPainter::WPainter()
    : _painter(new QPainter())
    , _allocated(true)
{

}

inline WPainter::WPainter(QPdfWriter *pdfWriter)
    : _painter(new QPainter())
    , _allocated(true)
{
    _painter->begin(pdfWriter);
}

inline void WPainter::begin(WImage *image)
{
    _painter->begin(image);
}

inline bool WPainter::begin(WPixmap *pixmap)
{
    return _painter->begin(pixmap);
}

inline void WPainter::drawLine(const PointF &p1, const PointF &p2)
{
    const QPointF qtPoint1 = {
            p1.x(),
            p1.y()
    };
    const QPointF qtPoint2 = {
            p2.x(),
            p2.y()
    };
    this->_painter->drawLine(qtPoint1, qtPoint2);
}

inline void WPainter::setPen(WPen &pen)
{
    this->_painter->setPen(pen._pen);
}

inline void WPainter::drawImage(const RectF &target, const WImage &image, const RectF &source)
{
    const auto convertToQt = [](const RectF &rect) -> QRectF {
        return {
                QPointF {
                        rect.topLeft().x(),
                        rect.topLeft().y()
                },
                QPointF {
                        rect.bottomRight().x(),
                        rect.bottomRight().y()
                }
        };
    };
    const auto qtTarget = convertToQt(target);
    const auto qtSource = convertToQt(source);

    this->_painter->drawImage(qtTarget, image, qtSource);
}

inline void WPainter::drawPixmap(const RectF &target, const WPixmap &pixmap, const RectF &source)
{
    constexpr auto convertToQt = [](const RectF &rect) -> QRectF {
        return {
                QPointF {
                        rect.topLeft().x(),
                        rect.topLeft().y()
                },
                QPointF {
                        rect.bottomRight().x(),
                        rect.bottomRight().y()
                }
        };
    };
    const auto qtTarget = convertToQt(target);
    const auto qtSource = convertToQt(source);

    this->_painter->drawPixmap(qtTarget, pixmap, qtSource);
}

inline void WPainter::drawPixmap(const RectF &target, const WPixmap &pixmap)
{
    const auto convertToQt = [](const RectF &rect) -> QRectF {
        return {
                QPointF {
                        rect.topLeft().x(),
                        rect.topLeft().y()
                },
                QPointF {
                        rect.bottomRight().x(),
                        rect.bottomRight().y()
                }
        };
    };
    const QRect qtTarget = convertToQt(target).toRect();

    this->_painter->drawPixmap(qtTarget, pixmap);
}

inline void WPainter::drawPixmap(const WRect &target, const WPixmap &pixmap)
{
    const auto convertToQt = [](const WRect &rect) -> QRect {
        return {
                QPoint {
                        rect.topLeft().x(),
                        rect.topLeft().y()
                },
                QPoint {
                        rect.bottomRight().x(),
                        rect.bottomRight().y()
                }
        };
    };
    const QRect qtTarget = convertToQt(target);

    this->_painter->drawPixmap(qtTarget, pixmap);
}

inline void WPainter::drawPixmap(const WRect &target, const WPixmap &pixmap, const WRect &source)
{
    const auto convertToQt = [](const WRect &rect) -> QRectF {
        return {
                QPoint {
                        rect.topLeft().x(),
                        rect.topLeft().y()
                },
                QPoint {
                        rect.bottomRight().x(),
                        rect.bottomRight().y()
                }
        };
    };
    const auto qtTarget = convertToQt(target);
    const auto qtSource = convertToQt(source);

    this->_painter->drawPixmap(qtTarget, pixmap, qtSource);
}

inline void WPainter::drawPoint(const PointF &point)
{
    this->_painter->drawPoint(QPointF {
        point.x(), point.y()
    });
}

inline void WPainter::drawRect(const RectF &rect)
{
    const auto convertToQt = [](const RectF &rect) -> QRectF {
        return {
                QPointF {
                        rect.topLeft().x(),
                        rect.topLeft().y()
                },
                QPointF {
                        rect.bottomRight().x(),
                        rect.bottomRight().y()
                }
        };
    };

    const auto qtRect = convertToQt(rect);

    _painter->drawRect(qtRect);
}

inline void WPainter::drawEllipse(const PointF &center, double rx, double ry)
{
    _painter->drawEllipse(QPointF {
        center.x(),
        center.y()
    }, rx, ry);
}

inline bool WPainter::isActive() const
{
    return this->_painter->isActive();
}

inline bool WPainter::end()
{
    return this->_painter->end();
}

inline WPainter::CompositionMode WPainter::compositionMode() const
{
    const auto c = _painter->compositionMode();

    switch (c) {
        case QPainter::CompositionMode::CompositionMode_Clear:
            return WPainter::CompositionMode::CompositionMode_Clear;
        case QPainter::CompositionMode::CompositionMode_SourceOver:
            return WPainter::CompositionMode::CompositionMode_SourceOver;
        case QPainter::CompositionMode::CompositionMode_DestinationOut:
            return WPainter::CompositionMode::CompositionMode_DestinationOver;
        case QPainter::CompositionMode_DestinationOver: break;
        case QPainter::CompositionMode_Source: break;
        case QPainter::CompositionMode_Destination: break;
        case QPainter::CompositionMode_SourceIn: break;
        case QPainter::CompositionMode_DestinationIn: break;
        case QPainter::CompositionMode_SourceOut: break;
        case QPainter::CompositionMode_SourceAtop: break;
        case QPainter::CompositionMode_DestinationAtop: break;
        case QPainter::CompositionMode_Xor: break;
        case QPainter::CompositionMode_Plus: break;
        case QPainter::CompositionMode_Multiply: break;
        case QPainter::CompositionMode_Screen: break;
        case QPainter::CompositionMode_Overlay: break;
        case QPainter::CompositionMode_Darken: break;
        case QPainter::CompositionMode_Lighten: break;
        case QPainter::CompositionMode_ColorDodge: break;
        case QPainter::CompositionMode_ColorBurn: break;
        case QPainter::CompositionMode_HardLight: break;
        case QPainter::CompositionMode_SoftLight: break;
        case QPainter::CompositionMode_Difference: break;
        case QPainter::CompositionMode_Exclusion: break;
        case QPainter::RasterOp_SourceOrDestination: break;
        case QPainter::RasterOp_SourceAndDestination: break;
        case QPainter::RasterOp_SourceXorDestination: break;
        case QPainter::RasterOp_NotSourceAndNotDestination: break;
        case QPainter::RasterOp_NotSourceOrNotDestination: break;
        case QPainter::RasterOp_NotSourceXorDestination: break;
        case QPainter::RasterOp_NotSource: break;
        case QPainter::RasterOp_NotSourceAndDestination: break;
        case QPainter::RasterOp_SourceAndNotDestination: break;
        case QPainter::RasterOp_NotSourceOrDestination: break;
        case QPainter::RasterOp_SourceOrNotDestination: break;
        case QPainter::RasterOp_ClearDestination: break;
        case QPainter::RasterOp_SetDestination: break;
        case QPainter::RasterOp_NotDestination: break;
    }
    W_ASSERT_TEXT(true, "not known composition mode " << (int) c);
    std::abort();
    return WPainter::CompositionMode::CompositionMode_DestinationOver;
}

inline void WPainter::setCompositionMode(WPainter::CompositionMode compositionMode)
{
    QPainter::CompositionMode qtCompositionMode;

    switch (compositionMode) {
        case WPainter::CompositionMode_Clear: qtCompositionMode = QPainter::CompositionMode_Clear;
        case WPainter::CompositionMode_DestinationOver: qtCompositionMode = QPainter::CompositionMode_DestinationOver;
        case WPainter::CompositionMode_SourceOver: qtCompositionMode = QPainter::CompositionMode_SourceOver;
    }

    this->_painter->setCompositionMode(qtCompositionMode);
}

inline void WPainter::drawLine(int x1, int y1, int x2, int y2)
{
    this->_painter->drawLine(x1, y1, x2, y2);
}

