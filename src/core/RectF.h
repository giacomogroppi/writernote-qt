//
// Created by Giacomo Groppi on 08/05/23.
//

#ifndef WRITERNOTE_RECTF_H
#define WRITERNOTE_RECTF_H

#include "Rect.h"
#include "core/PointF.h"

using RectF = RectTemplate<double>;


#ifdef USE_QT
# include <QDebug>

inline Q_CORE_EXPORT QDebug operator<<(QDebug d, const RectF &p)
{
    d.nospace() << "RectF(" << p.topLeft() << ',' << p.bottomRight() << ')';
    return d.space();
}
#endif //USE_QT

#endif //WRITERNOTE_RECTF_H
