#include "wline.h"
#include <QLine>

constexpr auto debug_WLine = true;

WLine::WLine(const WLine &line)
{
    this->_m = line._m;
    this->_pt1 = line._pt1;
    this->_pt2 = line._pt2;
}

WLine::WLine(const QPointF &pt1, const QPointF &pt2)
{
    _pt1 = pt1;
    _pt2 = pt2;

    const auto deltax = _pt1.x() - _pt2.x();
    const auto deltay = _pt1.y() - _pt2.y();

    _is_vertical = deltax == 0.;

    if(likely(!_is_vertical)){
        _m = deltay / deltax ;
    }

    _p = _pt1.y() - _pt1.x() * _m;

    W_ASSERT(_pt1.x() >= 0.);
    W_ASSERT(_pt1.y() >= 0.);
    W_ASSERT(_pt2.x() >= 0.);
    W_ASSERT(_pt2.y() >= 0.);

    if(!_is_vertical){
        W_ASSERT(qAbs(_m) != HUGE_VAL);
        W_ASSERT(qAbs(_p) != HUGE_VAL);
    }
}

bool WLine::belongs(const QPointF &point, cdouble precision) const
{
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
    W_ASSERT(vertical._pt1.x() == vertical._pt2.x());

    /*if(!(is_between_change(line.pt1().y(), vertical.pt1().y(), line.pt2().y()) or
        is_between_change(line.pt1().y(), vertical.pt2().y(), line.pt2().y())))
    {
        WDebug(debug_WLine, "Not in ")
    }*/



    const auto y = vertical.pt1().x() * line._m + line._p;
    const auto one = line.belongs(QPointF(vertical.pt1().x(), y), precision);
    const auto two = vertical.is_in_domain(QPointF(vertical.pt1().x(), y), precision);
    return one && two;
}

bool WLine::intersect(const WLine &line1, const WLine &line2, cint precision, QPointF *result)
{

    double x, y;
    bool touch;

    if(unlikely(line1._is_vertical && line2._is_vertical)){
        WDebug(debug_WLine, "Both line are vertical")
        return false;
    }

    /*if(!line1._is_vertical && !line2._is_vertical){
        if(is_near(line1._m, line2._m, 0.02)){
            WDebug(debug, qstr("No line vertical but two _m are close enough m1: %1 m2: %2").arg(line1._m).arg(line2._m));
            goto out;
            return false;
        }
    }*/
out:
    if(likely(!line2._is_vertical && !line1._is_vertical)) {
        WDebug(debug_WLine, "No line vertical");
        x = (line2._p - line1._p) / (line1._m - line2._m);
        y = line2._m * x + line2._p;

        touch = line2.is_in_domain(QPointF(x, y), precision);

        WDebug(debug_WLine, (touch ? "First in domain" : "First not in domain"));

        if(touch and line1.is_in_domain(QPointF(x, y), precision)){
            WDebug(debug_WLine, "Second in domain");
        }else{
            touch = false;
        }

    }else{
        WDebug(debug_WLine, (line2._is_vertical ? "First list vertical" : "Second line vertical"));
        if(line2._is_vertical){
            touch = WLine::intersect_vertical(line1, line2, precision);
        }else{
            touch =  WLine::intersect_vertical(line2, line1, precision);
        }
    }

    WDebug(debug_WLine, qstr("Are line intersect? %1").arg(touch));
    return touch;
}

bool WLine::is_in_domain(const QPointF& point, cdouble precision) const
{
    W_ASSERT(precision >= 0.);

    const auto x = point.x();
    const auto y = point.y();

    const auto xmin = qMin(_pt1.x(), _pt2.x());
    const auto xmax = qMax(_pt1.x(), _pt2.x());
    const auto ymax = qMax(_pt1.y(), _pt2.y());
    const auto ymin = qMin(_pt1.y(), _pt2.y());

    return  is_between(xmin - precision, x, xmax + precision) and 
            is_between(ymin - precision, y, ymax + precision);
}
