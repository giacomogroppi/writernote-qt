#ifndef WLINE_H
#define WLINE_H

#include "utils/common_script.h"
#include <QPointF>
#include <QPoint>
#include <QRectF>

class WLine
{
private:
    double _m, _p;
    double _xt, _yt;
    double _xb, _yb;
    bool _is_vertical;

    static bool intersect_vertical(const WLine &line, const WLine &vertical, cdouble precision);
    bool belongs(const QPointF &point, cdouble precision) const;

public:
    WLine() = default;
    WLine(const WLine &line);
    WLine(const QPointF &topLeft, const QPointF &bottomRigth);
    WLine(cdouble xt, cdouble yt, cdouble xb, cdouble yb);

    bool intersect(const WLine &line, cint precision) const;
    bool is_in_domain(const QPointF& point, cdouble precision) const;
    QRectF toRect() const;

    void get_point(QPointF &tl, QPointF &br) const;

    WLine &operator=(const WLine &other);
};

force_inline WLine::WLine(cdouble xt, cdouble yt, cdouble xb, cdouble yb)
{
    W_ASSERT(xt <= xb);
    W_ASSERT(yt <= yb);
    *this = WLine(QPoint(xt, yt), QPointF(xb, yb));
}

force_inline void WLine::get_point(QPointF &tl, QPointF &br) const
{
    W_ASSERT(_xt >= 0.);
    W_ASSERT(_yt >= 0.);
    W_ASSERT(_xb >= 0.);
    W_ASSERT(_yb >= 0.);

    tl = QPointF(_xt, _yt);
    br = QPointF(_xb, _yb);
}

force_inline QRectF WLine::toRect() const
{
    return QRectF(QPointF(_xt, _yt), QPointF(_xb, _yb));
}

force_inline WLine &WLine::operator=(const WLine &other)
{
    if(unlikely(&other == this)){
        return *this;
    }
    memcpy(this, &other, sizeof(*this));

    return *this;
}

#endif // WLINE_H
