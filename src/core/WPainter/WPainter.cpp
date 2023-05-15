#include "WPainter.h"


#if defined(USE_QT)

WPainter::WPainter(QPaintDevice * dev)
    : QPainter(dev)
{
}

#endif // USE_QT
