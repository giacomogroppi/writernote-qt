//
// Created by Giacomo Groppi on 01/05/23.
//

#ifndef WRITERNOTE_POINTF_H
#define WRITERNOTE_POINTF_H

#include "PointTemplate.h"
#ifdef USE_QT
# include <QDebug>
#endif // USE_QT

using PointF = PointTemplate<double>;

#ifdef USE_QT

inline Q_CORE_EXPORT QDebug operator<<(QDebug d, const PointF &p)
{
    d.nospace() << "PointF(" << p.x() << ',' << p.y() << ')';
    return d.space();
}

#endif // USE_QT

#endif //WRITERNOTE_POINTF_H
