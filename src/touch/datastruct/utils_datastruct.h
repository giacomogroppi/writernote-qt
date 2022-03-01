#ifndef UTILS_DATASTRUCT_H
#define UTILS_DATASTRUCT_H

#include "utils/common_script.h"
#include <QRectF>
#include <QPointF>

force_inline QRectF datastruct_rect(const QPointF &p1, const QPointF &p2)
{
    double xt, yt, xb, yb;

    if(p1.x() < p2.x()){
        xt = p1.x();
        xb = p2.x();
    }else{
        xt = p2.x();
        xb = p1.x();
    }

    if(p1.y() < p2.y()){
        yt = p1.y();
        yb = p2.y();
    }
    else{
        yt = p2.y();
        yb = p2.y();
    }

    W_ASSERT(xt <= xb);
    W_ASSERT(yt <= yb);

    return QRectF(xt, yt, xb, yb);
}

#endif // UTILS_DATASTRUCT_H
