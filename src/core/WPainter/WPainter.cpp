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

#endif // USE_QT
