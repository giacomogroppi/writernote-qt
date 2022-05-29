#ifndef WLINE_H
#define WLINE_H

#include "utils/common_script.h"
#include "touch/datastruct/utils_datastruct.h"
#include <QPointF>
#include <QPoint>
#include <QRectF>

class WLine
{
private:
    double _m, _p;
    QPointF _pt1, _pt2;
    bool _is_vertical;

    static bool intersect_vertical(const WLine &line, const WLine &vertical, cdouble precision);
    bool belongs(const QPointF &point, cdouble precision) const;

    const QPointF &pt1() const;
    const QPointF &pt2() const;

public:
    WLine() = default;
    WLine(const WLine &line);
    WLine(const QPointF &topLeft, const QPointF &bottomRigth);
    WLine(cdouble xt, cdouble yt, cdouble xb, cdouble yb);

    static bool intersect(const WLine &line1, const WLine &line2, int precision, QPointF *result = NULL);
    bool is_in_domain(const QPointF& point, cdouble precision) const;
    QRectF toRect() const;

    void get_point(QPointF &tl, QPointF &br) const;

    WLine &operator=(const WLine &other);
};

force_inline const QPointF &WLine::pt1() const
{
    return this->_pt1;
}

force_inline const QPointF &WLine::pt2() const
{
    return this->_pt2;
}

force_inline WLine::WLine(cdouble xt, cdouble yt, cdouble xb, cdouble yb)
{
    *this = WLine(QPoint(xt, yt), QPointF(xb, yb));
}

force_inline void WLine::get_point(QPointF &pt_1, QPointF &pt_2) const
{
    W_ASSERT(pt1().y() >= 0.);
    W_ASSERT(pt1().x() >= 0.);
    W_ASSERT(pt2().y() >= 0.);
    W_ASSERT(pt2().x() >= 0.);

    pt_1 = _pt1;
    pt_2 = _pt2;
}

force_inline QRectF WLine::toRect() const
{
    return datastruct_rect(_pt1, _pt2);
}

force_inline WLine &WLine::operator=(const WLine &other)
{
    if(unlikely(&other == this)){
        return *this;
    }

    this->_m = other._m;
    this->_p = other._p;
    this->_is_vertical = other._is_vertical;
    this->_pt1 = other._pt1;
    this->_pt2 = other._pt2;

    return *this;
}

#endif // WLINE_H
