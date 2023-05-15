#include "WPainter.h"


#if defined(USE_QT)

WPainter::WPainter(QPaintDevice * dev)
    : WPainter(dev)
{
}

#endif // USE_QT
