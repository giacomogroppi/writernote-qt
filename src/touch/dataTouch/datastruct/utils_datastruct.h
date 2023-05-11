#pragma once

#include "utils/WCommonScript.h"
#include "core/Rect.h"
#include "core/RectF.h"

typedef float pressure_t;

force_inline RectF datastruct_rect(const PointF &p1, const PointF &p2)
{
    double xt, yt, xb, yb;

    if (p1.x() < p2.x()) {
        xt = p1.x();
        xb = p2.x();
    } else {
        xt = p2.x();
        xb = p1.x();
    }

    if (p1.y() < p2.y()) {
        yt = p1.y();
        yb = p2.y();
    } else {
        yt = p2.y();
        yb = p2.y();
    }

    W_ASSERT(xt <= xb);
    W_ASSERT(yt <= yb);

    return RectF(xt, yt, xb, yb);
}

bool datastruct_isinside(const PointF &topleft, const PointF &bottonright, const class Stroke &stroke);

force_inline bool datastruct_isinside(const PointF &topleft, const PointF &bottonright, const PointF &point)
{
    W_ASSERT(topleft.x() <= bottonright.x());
    W_ASSERT(topleft.y() <= bottonright.y());
    return RectF(topleft, bottonright).contains(point);
}

force_inline bool datastruct_isinside(const RectF &rect, const PointF &point)
{
    return datastruct_isinside(rect.topLeft(), rect.bottomRight(), point);
}
