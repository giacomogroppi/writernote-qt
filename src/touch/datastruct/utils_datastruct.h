#pragma once

#include "utils/common_script.h"
#include <QRectF>
#include <QPointF>

typedef float pressure_t;

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

bool datastruct_isinside(const QPointF &topleft, const QPointF &bottonright, const class stroke &stroke);

force_inline bool datastruct_isinside(const QPointF &topleft, const QPointF &bottonright, const QPointF &point)
{
    W_ASSERT(topleft.x() <= bottonright.x());
    W_ASSERT(topleft.y() <= bottonright.y());
    return QRectF(topleft, bottonright).contains(point);
}

force_inline bool datastruct_isinside(const QRectF &rect, const QPointF &point)
{
    return datastruct_isinside(rect.topLeft(), rect.bottomRight(), point);
}
