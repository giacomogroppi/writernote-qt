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

    const auto deltay = _yt - _yb;
    _is_vertical = deltay == 0.;

    if(likely(!_is_vertical)){
        _m = (_xt - _xb) / deltay;
    }

    _p = _yb - _xb * _m;

    W_ASSERT(_xt <= _xb);
    W_ASSERT(_xt >= 0.);
    W_ASSERT(_yb >= 0.);
    W_ASSERT(_yt <= _yb);
}

bool WLine::intersect_vertical(const WLine &line, const WLine &vertical, cdouble precision)
{
    W_ASSERT(vertical._is_vertical);
    W_ASSERT(!line._is_vertical);
    std::abort();
}

bool WLine::intersect(const WLine &line, cint precision) const
{
    double x, y;

    if(unlikely(line._is_vertical && this->_is_vertical))
        return false;

    if(is_near(line._m, _m, 0.02))
        return false;

    if(likely(!_is_vertical && line._is_vertical)) {
            x = (this->_p - line._p) / (line._m - this->_m);
            y = _m * x + _p;

            return is_in_domain(QPointF(x, y), precision) &&
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

    return  _xb + precision >= point.x() &&
            _xt <= point.x() + precision &&
            _yb + precision >= point.y() &&
            _yt <= point.y() + precision;
}
