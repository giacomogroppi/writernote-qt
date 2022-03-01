#include "wline.h"

WLine::WLine(const WLine &line)
{
    this->_m = line._m;
    this->_xb = line._xb;
    this->_xt = line._xt;
    this->_yb = line._yb;
    this->_yt = line._yt;
}

WLine::WLine(const QPointF &topLeft, const QPointF &bottomRigth)
{
    _xt = qMin(topLeft.x(), bottomRigth.x());
    _xb = qMax(topLeft.x(), bottomRigth.x());

    _yt = qMin(topLeft.y(), bottomRigth.y());
    _yb = qMax(topLeft.y(), bottomRigth.y());

    _m = (_xt - _xb) / (_yt  - _yb);
    _p = _yb - _xb * _m; 
}

bool WLine::intersect(const WLine &line, cint precision)
{
    double x, y;

    x = (this->_p - line._p) / (line._m - this->_m);
    y = _m * x + _p;

    return      is_in_domain(QPointF(x, y), precision) &&
           line.is_in_domain(QPointF(x, y), precision);
}

bool WLine::is_in_domain(const QPointF& point, cdouble precision) const
{
    W_ASSERT(precision >= 0.);
    W_ASSERT(_xb >= _xt);
    W_ASSERT(_yb >= _yt);

    cint c1 = _xb + precision >= point.x();
    cint c2 = _xt <= point.x() + precision;
    cint c3 = _yb + precision >= point.y();
    cint c4 = _yt <= point.y() + precision;

    return c1 & c2 & c3 & c4;
}
