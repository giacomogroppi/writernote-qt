//
// Created by Giacomo Groppi on 08/05/23.
//

#include "WPen.h"

WPen::WPen()
{
#ifdef USE_QT
    this->_pen = QPen(QBrush(), 1.0, Qt::SolidLine, Qt::MPenCapStyle, Qt::RoundJoin);
#else
#endif
}

#ifdef USE_QT
void WPen::setBrush(const QBrush &brush)
{
    this->_pen.setBrush(brush);
}
#endif // USE_QT
