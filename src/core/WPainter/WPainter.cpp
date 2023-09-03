#include "WPainter.h"

#if defined(USE_QT)

WPainter::~WPainter()
{
    if (_allocated) {
        delete this->_painter;
    }
}

void WPainter::setAntialeasing()
{
    _painter->setRenderHints(QPainter::Antialiasing);
    _painter->setRenderHint(QPainter::SmoothPixmapTransform);
    _painter->setRenderHint(QPainter::TextAntialiasing);
}

void WPainter::setBrush(const QBrush &brush)
{
    this->_painter->setBrush(brush);
}

void WPainter::move(const PointF &point) noexcept
{
    _path.moveTo(QPointF(point.x(), point.y()));
}

void WPainter::addCurve(const PointF &to, const PointF &control) noexcept
{
    _path.lineTo({to.x(), to.y()});
}

void WPainter::closePath () noexcept
{
    _painter->drawPath(_path);
    _path = QPainterPath();
}

#endif // USE_QT
