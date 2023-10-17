#pragma once

#include "utils/WCommonScript.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "ExactNumber/ExactNumber.h"

template <class Precision = double>
class WLineTemplate
{
private:
    static constexpr auto debug_WLine = true;
    Precision _m, _p;
    PointTemplate<Precision> _pt1, _pt2;
    bool _is_vertical;

    static bool intersect_vertical(const WLineTemplate &line, const WLineTemplate &vertical, Precision precision);

    bool isVertical() const;

public:
    WLineTemplate() = default;
    WLineTemplate(WLineTemplate<Precision> &&other) noexcept = default;
    WLineTemplate(const WLineTemplate<Precision> &line);
    WLineTemplate(const PointTemplate<Precision> &topLeft, const PointTemplate<Precision> &bottomRight);
    WLineTemplate(Precision xt, Precision yt, Precision xb, Precision yb);

    auto top (Precision amount) const -> WLineTemplate<Precision>;
    auto bottom (Precision amount) const -> WLineTemplate<Precision>;
    auto left (Precision amount) const -> WLineTemplate<Precision>;
    auto right (Precision amount) const -> WLineTemplate<Precision>;

    auto addTop(Precision precision) const -> WLineTemplate<Precision>;
    auto addLeft(Precision precision) const -> WLineTemplate<Precision>;
    auto addRight(Precision precision) const -> WLineTemplate<Precision>;
    auto addBottom(Precision precision) const -> WLineTemplate<Precision>;

    /**
     * \return A new WLineTemplate grow left with the same _m
     * */
    auto growLeft(Precision amount) const -> WLineTemplate<Precision>;
    auto growRight(Precision amount) const -> WLineTemplate<Precision>;

    static bool intersect(
            const WLineTemplate<Precision> &line1,
            const WLineTemplate<Precision> &line2,
            Precision precision, Precision *result = nullptr);

    /**
     * By definition if the line is vertical and precision is 0. the return value is always false
     * \param precision The precision of the search
     * \return True if the parameter passed as an argument is inside the rectangle formed by the colon of the ray
     * */
    bool isInDomain(const PointTemplate<Precision>& point, Precision precision) const;
    RectTemplate<Precision> toRect() const;

    void get_point(PointTemplate<Precision> &tl, PointTemplate<Precision> &br) const;

    WLineTemplate<Precision> &operator=(const WLineTemplate<Precision> &other);

    bool belongs(const PointTemplate<Precision> &point, Precision precision) const;

    auto operator==(const WLineTemplate<Precision> &other) const -> bool;

    auto isUp(const PointTemplate<Precision> &point) const -> bool;
    auto isDown(const PointTemplate<Precision> &point) const -> bool;

    auto pointAt(Precision x) const -> Precision;

    auto pt1() const -> const PointTemplate<Precision> &;
    auto pt2() const -> const PointTemplate<Precision> &;

#ifdef USE_QT
    friend QDebug operator<<(QDebug d, const WLineTemplate<> &p);
#endif // USE_QT
};

template<class Precision>
inline auto WLineTemplate<Precision>::growLeft(Precision amount) const -> WLineTemplate<Precision>
{
    W_ASSERT(amount >= 0);
    //W_ASSERT(!isVertical());

    const auto isPt1 = _pt1.x() < _pt2.x();
    const auto minX = isPt1 ? _pt1.x() : _pt2.x();
    const auto shouldBe = this->pointAt(minX - amount);

    return {
            {minX - amount, shouldBe},
            isPt1 ? _pt2 : _pt1
    };
}

template<class Precision>
inline auto WLineTemplate<Precision>::growRight(Precision amount) const -> WLineTemplate<Precision>
{
    W_ASSERT(amount >= 0);
    //W_ASSERT(!isVertical());

    const auto isPt1 = _pt1.x() > _pt2.x();
    const auto maxX = isPt1 ? _pt1.x() : _pt2.x();
    const auto shouldBe = this->pointAt(maxX + amount);

    return {
            isPt1 ? _pt2 : _pt1,
            {maxX + amount, shouldBe}
    };
}

template <class Precision>
inline auto WLineTemplate<Precision>::pointAt(Precision x) const -> Precision
{
    return _m * x + _p;
}

template <class Precision>
inline auto WLineTemplate<Precision>::isUp(const PointTemplate<Precision> &point) const -> bool
{
    W_ASSERT (!isVertical());

    const auto myY = pointAt(point.x());
    return point.y() <= myY;
}

template <class Precision>
inline auto WLineTemplate<Precision>::isDown(const PointTemplate<Precision> &point) const -> bool
{
    W_ASSERT (!isVertical());

    const auto myY = pointAt(point.x());
    return point.y() >= myY;
}

template <class Precision>
inline auto WLineTemplate<Precision>::addBottom(Precision precision) const -> WLineTemplate<Precision>
{
    if (_pt1.y() > _pt2.y) {
        return {
            _pt1.bottom(precision),
            _pt2
        };
    } else {
        return {
            _pt1,
            _pt2.bottom(precision)
        };
    }
}

template <class Precision>
inline auto WLineTemplate<Precision>::addLeft(Precision precision) const -> WLineTemplate<Precision>
{
    if (_pt1.x() < _pt2.x()) {
        return {
            _pt1.left(precision),
            _pt2
        };
    } else {
        return {
            _pt1,
            _pt2.left(precision)
        };
    }
}

template <class Precision>
inline auto WLineTemplate<Precision>::addRight(Precision precision) const -> WLineTemplate<Precision>
{
    if (_pt1.x() > _pt2.x()) {
        return {
                _pt1.right(precision),
                _pt2
        };
    } else {
        return {
                _pt1,
                _pt2.right(precision)
        };
    }
}

template <class Precision>
inline auto WLineTemplate<Precision>::addTop(Precision precision) const -> WLineTemplate<Precision>
{
    if (_pt1.y() < _pt2.y()) {
        return {
                _pt1.top(precision),
                _pt2
        };
    } else {
        return {
                _pt1,
                _pt2.top(precision)
        };
    }
}

template <class Precision>
inline auto WLineTemplate<Precision>::top(Precision amount) const -> WLineTemplate<Precision>
{
    return {
        this->_pt1.top(amount),
        this->_pt2.top(amount)
    };
}

template <class Precision>
inline auto WLineTemplate<Precision>::bottom(Precision amount) const -> WLineTemplate<Precision>
{
    return {
        this->_pt1.bottom(amount),
        this->_pt2.bottom(amount)
    };
}

template <class Precision>
inline auto WLineTemplate<Precision>::left(Precision amount) const -> WLineTemplate<Precision>
{
    return {
        this->_pt1.left(amount),
        this->_pt2.left(amount)
    };
}

template <class Precision>
inline auto WLineTemplate<Precision>::right(Precision amount) const -> WLineTemplate<Precision>
{
    return {
        this->_pt1.right(amount),
        this->_pt2.right(amount)
    };
}

template <class Precision>
inline bool WLineTemplate<Precision>::isVertical() const
{
    return this->_is_vertical;
}

template <class Precision>
force_inline const PointTemplate<Precision> &WLineTemplate<Precision>::pt1() const
{
    return this->_pt1;
}

template <class Precision>
inline const PointTemplate<Precision> &WLineTemplate<Precision>::pt2() const
{
    return this->_pt2;
}

template <class Precision>
force_inline WLineTemplate<Precision>::WLineTemplate(Precision xt, Precision yt, Precision xb, Precision yb)
{
    *this = WLineTemplate(PointTemplate<Precision> (xt, yt), PointTemplate<Precision>(xb, yb));
}

template <class Precision>
inline void WLineTemplate<Precision>::get_point(
        PointTemplate<Precision> &pt_1,
        PointTemplate<Precision> &pt_2) const
{
    W_ASSERT(pt1().y() >= 0.);
    W_ASSERT(pt1().x() >= 0.);
    W_ASSERT(pt2().y() >= 0.);
    W_ASSERT(pt2().x() >= 0.);

    pt_1 = _pt1;
    pt_2 = _pt2;
}

template <class Precision>
inline RectTemplate<Precision> WLineTemplate<Precision>::toRect() const
{
    return datastruct_rect(_pt1, _pt2);
}

template <class Precision>
inline WLineTemplate<Precision> &WLineTemplate<Precision>::operator=(const WLineTemplate<Precision> &other)
{
    if (&other == this) {
        return *this;
    }

    this->_m = other._m;
    this->_p = other._p;
    this->_is_vertical = other._is_vertical;
    this->_pt1 = other._pt1;
    this->_pt2 = other._pt2;

    return *this;
}

template <class Precision>
inline WLineTemplate<Precision>::WLineTemplate(const WLineTemplate<Precision> &line)
{
    this->_m = line._m;
    this->_pt1 = line._pt1;
    this->_pt2 = line._pt2;
    this->_is_vertical = line._is_vertical;
    this->_p = line._p;
}

template <class Precision>
inline WLineTemplate<Precision>::WLineTemplate(
        const PointTemplate<Precision> &pt1,
        const PointTemplate<Precision> &pt2)
{
    _pt1 = pt1;
    _pt2 = pt2;

    const Precision dx = _pt1.x() - _pt2.x();
    const Precision dy = _pt1.y() - _pt2.y();

    _is_vertical = (dx == Precision((int) 0));

    if (!_is_vertical) {
        _m = dy / dx ;
    }

    _p = _pt1.y() - _pt1.x() * _m;

    W_ASSERT(_pt1.x() >= Precision(0));
    W_ASSERT(_pt1.y() >= Precision(0));
    W_ASSERT(_pt2.x() >= Precision(0));
    W_ASSERT(_pt2.y() >= Precision(0));
}

// ritorna true se il punto appartiene alla linea
template <class Precision>
inline bool WLineTemplate<Precision>::belongs(const PointTemplate<Precision> &point, Precision precision) const
{
    W_ASSERT(!isVertical());

    // new implementation
    const auto isUp = this->isUp(point.top(precision));
    const auto isDown = this->isDown(point.bottom(precision));

    return isUp and isDown and isInDomain(point, precision);

    /*
    const auto res = WCommonScript::is_near(this->_m * point.x() + this->_p, point.y(), precision);

    if (isInDomain(point, precision)) {
        WDebug(debug_WLine, "\t" << "It's in domain" << point);
        if(res)
            return true;
    }else{
        WDebug(debug_WLine, "\t" << "It's not in domain" << point << _pt1 << _pt2);
    }

    return false;
     */
}

template <class Precision>
inline bool WLineTemplate<Precision>::intersect_vertical(
        const WLineTemplate<Precision> &line,
        const WLineTemplate<Precision> &vertical,
        Precision precision)
{
    W_ASSERT(vertical.isVertical());
    W_ASSERT(!line.isVertical());
    W_ASSERT(vertical._pt1.x() == vertical._pt2.x());

    const Precision xTouch = vertical.pt1().x();

    const Precision y = xTouch * line._m + line._p;

    const bool AreTouch = line.belongs(
            {xTouch, y},
            precision
    );

    const bool IsInDomain = vertical.isInDomain(
            {xTouch, y},
            precision
    );

    return AreTouch and IsInDomain;
}

template <class Precision>
bool WLineTemplate<Precision>::intersect(
        const WLineTemplate<Precision> &line1,
        const WLineTemplate<Precision> &line2,
        Precision fakePrecision,
        Precision *)
{
    const auto precision = fakePrecision / 2;
    // TODO: the domain [return false is false in some cases]
    if (line1.isVertical() && line2.isVertical())
        return false;

    const auto isTouch = [](
                const WLineTemplate<Precision> &line1,
                const WLineTemplate<Precision> &line2
            ) -> bool {

        if (!line2.isVertical() and !line1.isVertical()) {
            WDebug(debug_WLine, "No line vertical");
            const auto x = (line2._p - line1._p) / (line1._m - line2._m);
            const auto y = line2._m * x + line2._p;

            const auto isInDomain = line2.isInDomain({x, y}, Precision(0));

            WDebug(debug_WLine, "Possible point of touch is" << PointTemplate<Precision>(x, y));
            WDebug(debug_WLine, (isInDomain ? "First in domain" : "First not in domain"));

            if (isInDomain and line1.isInDomain({x, y}, Precision(0))) {
                WDebug(debug_WLine, "Second in domain");
                return true;
            } else {
                return false;
            }
        } else {
            WDebug(debug_WLine, (line2.isVertical() ? "First line vertical" : "Second line vertical"));

            bool result;
            if (line2.isVertical()) {
                result = WLineTemplate::intersect_vertical(line1, line2, Precision(0));
            } else {
                result =  WLineTemplate::intersect_vertical(line2, line1, Precision(0));
            }

            WDebug(debug_WLine, line1.pt1() << line1.pt2() << line2.pt1() << line2.pt2());
            return result;
        }

    };

    /*
    const WLineTemplate<Precision> line1Up      = line1.top(precision)      .addLeft(precision).addRight(precision);
    const WLineTemplate<Precision> line1Down    = line1.bottom(precision)   .addLeft(precision).addRight(precision);

    const WLineTemplate<Precision> line2Up      = line2.top(precision)      .addLeft(precision).addRight(precision);
    const WLineTemplate<Precision> line2Down    = line2.bottom(precision)   .addLeft(precision).addRight(precision);
    */

    const WLineTemplate<Precision> line1Up      = line1.top(precision)      .growLeft(precision).growRight(precision);
    const WLineTemplate<Precision> line1Down    = line1.bottom(precision)   .growLeft(precision).growRight(precision);

    const WLineTemplate<Precision> line2Up      = line2.top(precision)      .growLeft(precision).growRight(precision);
    const WLineTemplate<Precision> line2Down    = line2.bottom(precision)   .growLeft(precision).growRight(precision);

    /*
    W_ASSERT(WCommonScript::is_near(line1Up._m, line1._m, 1e-05));
    W_ASSERT(WCommonScript::is_near(line2Up._m, line2._m, 1e-05));

    W_ASSERT(WCommonScript::is_near(line1Up._m, line1Down._m, 1e-05));
    W_ASSERT(WCommonScript::is_near(line2Up._m, line2Down._m, 1e-05));
    */

    if (isTouch(line1Up, line2Up))
        return true;
    if (isTouch(line1Up, line2Down))
        return true;
    if (isTouch(line1Down, line2Up))
        return true;
    if (isTouch(line1Down, line2Down))
        return true;

    return false;
}

template <class Precision>
inline bool WLineTemplate<Precision>::isInDomain(
        const PointTemplate<Precision>& point,
        Precision precision) const
{
    W_ASSERT(precision >= Precision(0));

    return RectTemplate<Precision> {
            _pt1,
            _pt2
    }.contains(point, precision);
}

#ifdef USE_QT
template <class Precision>
inline Q_CORE_EXPORT QDebug operator<<(QDebug d, const WLineTemplate<Precision> &p)
{
    d.space() << "WLine(";
    d.nospace() << p._pt1 << ", " << p._pt2;
    return d.nospace() << ")";
}
#endif // USE_QT

using WLine = WLineTemplate<double>;