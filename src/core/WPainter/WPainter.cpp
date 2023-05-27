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

#endif // USE_QT
