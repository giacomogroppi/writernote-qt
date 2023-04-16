#include "WPainter.h"


#ifndef Q_OS_IOS

WPainter::WPainter(QPaintDevice * dev)
    : QPainter(dev)
{
}

#endif // Q_OS_IOS
