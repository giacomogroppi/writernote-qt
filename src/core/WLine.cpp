#include "WLine.h"
#include "core/String/WString.h"

constexpr auto debug_WLine = false;

WLine::WLine(const WLine &line)
{
    this->_m = line._m;
    this->_pt1 = line._pt1;
    this->_pt2 = line._pt2;
    this->_is_vertical = line._is_vertical;
    this->_p = line._p;
}

WLine::WLine(const PointF &pt1, const PointF &pt2)
{
    _pt1 = pt1;
    _pt2 = pt2;

    const auto deltax = _pt1.x() - _pt2.x();
    const auto deltay = _pt1.y() - _pt2.y();

    _is_vertical = deltax == 0.;

    if (!_is_vertical) {
        _m = deltay / deltax ;
    }

    _p = _pt1.y() - _pt1.x() * _m;

    W_ASSERT(_pt1.x() >= 0.);
    W_ASSERT(_pt1.y() >= 0.);
    W_ASSERT(_pt2.x() >= 0.);
    W_ASSERT(_pt2.y() >= 0.);

    if(!_is_vertical){
        W_ASSERT(std::abs(_m) != HUGE_VAL);
        W_ASSERT(std::abs(_p) != HUGE_VAL);
    }
}

// ritorna true se il punto appartiene alla linea
bool WLine::belongs(const PointF &point, cdouble precision) const
{
    W_ASSERT(!this->_is_vertical);
    const auto res = WCommonScript::is_near(this->_m * point.x() + this->_p, point.y(), precision);

    WDebug(debug_WLine, "\t" << "Result" << res << qstr("m %1 point.x() %2 _p %3 point.y() %4 precision %5").arg(_m).arg(point.x()).arg(_p).arg(point.y()).arg(precision).toStdString());

    if(is_in_domain(point, precision)){
        WDebug(debug_WLine, "\t" << "It's in domain");
        if(res)
            return true;
    }else{
        WDebug(debug_WLine, "\t" << "It's not in domain");
    }

    return false;
}

bool WLine::intersect_vertical(const WLine &line, const WLine &vertical, cdouble precision)
{
    W_ASSERT(vertical._is_vertical);
    W_ASSERT(!line._is_vertical);
    W_ASSERT(vertical._pt1.x() == vertical._pt2.x());

    const double xTouch = vertical.pt1().x();

    const double y = xTouch * line._m + line._p;
    
    cbool AreTouch = line.belongs(
                            PointF(xTouch, y),
                            precision
                        );

    cbool IsInDomain = vertical.is_in_domain(
                PointF(xTouch, y),
                precision
            );

    return AreTouch and IsInDomain;
}

bool WLine::intersect(const WLine &line1, const WLine &line2, int precision, PointF *result)
{
    double x, y;
    bool touch;

    (void) result;
    //(void)(result);

    if (un(line1._is_vertical and line2._is_vertical)) {
        WDebug(debug_WLine, "Both line are vertical")
        return false;
    }

    if (!line2._is_vertical and !line1._is_vertical) {
        WDebug(debug_WLine, "No line vertical");
        x = (line2._p - line1._p) / (line1._m - line2._m);
        y = line2._m * x + line2._p;

        touch = line2.is_in_domain(PointF(x, y), precision);

        WDebug(debug_WLine, (touch ? "First in domain" : "First not in domain"));

        if (touch and line1.is_in_domain(PointF(x, y), precision)) {
            WDebug(debug_WLine, "Second in domain");
        } else {
            touch = false;
        }

    }else{
        WDebug(debug_WLine, (line2._is_vertical ? "First line vertical" : "Second line vertical"));
        if (line2._is_vertical) {
            touch = WLine::intersect_vertical(line1, line2, precision);
        } else {
            touch =  WLine::intersect_vertical(line2, line1, precision);
        }

        WDebug(debug_WLine, line1.pt1() << line1.pt2() << line2.pt1() << line2.pt2());
    }

    WDebug(debug_WLine, qstr("Are line intersect? %1").arg(touch ? "Yes" : "No"));
    return touch;
}

bool WLine::is_in_domain(const PointF& point, cdouble precision) const
{
    W_ASSERT(precision >= 0.);

    bool check;
    const auto real_precision = precision / 2.;

    const auto x = point.x();
    const auto y = point.y();

    const auto ymax = std::max(_pt1.y(), _pt2.y());
    const auto ymin = std::min(_pt1.y(), _pt2.y());

    if (!_is_vertical) {
        const auto xmin = std::min(_pt1.x(), _pt2.x());
        const auto xmax = std::max(_pt1.x(), _pt2.x());

        check = WCommonScript::is_between(xmin - real_precision, x, xmax + real_precision);
        WDebug(debug_WLine, "Line not vertical" << (check ? "in domain [x]" : "not in domain [x]"));

        if(WCommonScript::is_between(ymin - real_precision, y, ymax + real_precision)){
            WDebug(debug_WLine, "Line not vertical" << (check ? "in domain [y]" : "not in domain [y]"));                    
        }else{
            WDebug(debug_WLine, "Line not vertical" << (check ? "in domain [y]" : "not in domain [y]"));
            if(check)
                check = false;
        }

    } else {
        check = WCommonScript::is_near(this->pt1().x(), x, precision);

        WDebug(debug_WLine, (check ? "Line vertical is in domain [x]" : "Line vertical is not in domain [x]") << 
                "x_vertical" << pt1() << "x_point" << x << "Precision" << precision);

        check = check and WCommonScript::is_between(ymin - real_precision, y, ymax + real_precision);
        WDebug(debug_WLine, (check ? "Line vertical is in domain [y]" : "Line vertical is not in domain [y]") << 
                "y_min_vertical" << ymin << "y_max_vertical" << ymax << "y_point" << y);
    }

    return check;
}
