#include "wline.h"
#include <QLine>

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

    const auto deltax = _xt - _xb;
    const auto deltay = _yt - _yb;
    _is_vertical = deltax == 0.;

    if(likely(!_is_vertical)){
        _m = deltay / deltax ;
    }

    _p = _yb - _xb * _m;

    W_ASSERT(_xt <= _xb);
    W_ASSERT(_xt >= 0.);
    W_ASSERT(_yb >= 0.);
    W_ASSERT(_yt <= _yb);

    if(!_is_vertical){
        W_ASSERT(qAbs(_m) != HUGE_VAL);
        W_ASSERT(qAbs(_p) != HUGE_VAL);
    }
}

bool WLine::belongs(const QPointF &point, cdouble precision) const
{
    constexpr not_used const auto debug = true;

    W_ASSERT(!this->_is_vertical);

    const auto res = is_near(this->_m * point.x() + this->_p, point.y(), precision);

    if(res && is_in_domain(point, precision)){
        return true;
    }

    return false;
}

bool WLine::intersect_vertical(const WLine &line, const WLine &vertical, cdouble precision)
{
    W_ASSERT(vertical._is_vertical);
    W_ASSERT(!line._is_vertical);
    W_ASSERT(vertical._xb == vertical._xt);

    const auto y = vertical._xb * line._m + line._p;
    const auto one = line.belongs(QPointF(vertical._xb, y), precision);
    const auto two = vertical.is_in_domain(QPointF(vertical._xb, y), precision);
    return one && two;
}

bool WLine::intersect(const WLine &line, cint precision) const
{
    double x, y;

    if(unlikely(line._is_vertical && this->_is_vertical))
        return false;

    if(!line._is_vertical && !_is_vertical)
        if(is_near(line._m, _m, 0.02))
            return false;

    if(likely(!_is_vertical && !line._is_vertical)) {
            x = (this->_p - line._p) / (line._m - this->_m);
            y = _m * x + _p;

            return      is_in_domain(QPointF(x, y), precision) &&
                   line.is_in_domain(QPointF(x, y), precision);
    }else{
        if(this->_is_vertical){
            return WLine::intersect_vertical(line, *this, precision);
        }else{
            return WLine::intersect_vertical(*this, line, precision);
        }
    }
}

bool WLine::is_in_domain(const QPointF& point, cdouble precision) const
{
    W_ASSERT(precision >= 0.);
    W_ASSERT(_xb >= _xt);
    W_ASSERT(_yb >= _yt);

    const auto x = point.x();
    const auto y = point.y();

    return  _xb + precision >= x &&
            _xt - precision <= x &&
            _yb + precision >= y &&
            _yt - precision <= y;
}
