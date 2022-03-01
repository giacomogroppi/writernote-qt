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

    return this->is_in_domain(QPointF(x, y), precision) && line.is_in_domain(QPointF(x, y), precision);
}

bool WLine::is_in_domain(const QPointF& point, cint precision) const
{
    W_ASSERT(precision >= 0);
    cbool x = _xb + precision >= point.x() && point.x() <= _xt - precision;
    cbool y = _yb + precision >= point.y() && point.y() <= _yt - precision;
    return x && y;
}
