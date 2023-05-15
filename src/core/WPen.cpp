//
// Created by Giacomo Groppi on 08/05/23.
//

#include "WPen.h"

WPen::WPen()
{
#ifdef USE_QT
    QPen pen(QBrush(), 1.0, Qt::SolidLine, Qt::MPenCapStyle, Qt::RoundJoin);
#else
#endif
}
