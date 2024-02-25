#pragma once

#include "core/PointTemplate.h"
#include "core/WSizeTemplate.h"
#include "utils/WCommonScript.h"

template<typename T>
class RectTemplate {
private:
    PointTemplate<T> _topLeft;
    PointTemplate<T> _bottomRight;
public:
    constexpr RectTemplate() = default;
    constexpr RectTemplate(const RectTemplate<T> &other);
    constexpr RectTemplate(const PointTemplate<T> &pt1, const PointTemplate<T> &pt2);
    constexpr explicit RectTemplate(const WSizeTemplate<T> &size);
    constexpr RectTemplate(T xTopLeft, T yTopLeft, T width, T height);
    constexpr RectTemplate(const PointTemplate<T>&point, const WSizeTemplate<T> &size);

    constexpr auto addY(T y) const -> RectTemplate<T>;

    constexpr T top() const;
    constexpr T bottom() const;
    constexpr T left() const;
    constexpr T right() const;

    auto topLeft() const -> const PointTemplate<T> &;
    auto topRight() const -> const PointTemplate<T>;
    auto bottomLeft() const -> const PointTemplate<T>;
    auto bottomRight() const -> const PointTemplate<T> &;

    constexpr auto left(T amount) -> RectTemplate<T> &;
    constexpr auto top(T amount) -> RectTemplate<T> &;
    constexpr auto bottom(T amount) -> RectTemplate<T> &;
    constexpr auto right(T amount) -> RectTemplate<T> &;

    auto setHeight(T height) -> RectTemplate<T>&;
    auto setWidth(T width) -> RectTemplate<T>&;

    auto height() const -> T;
    auto width() const  -> T;
    
    auto setY(T y) -> RectTemplate<T>&;
    auto setX(T x) -> RectTemplate<T>&;

    void translate(const T &x, const T& y);

    auto setTopLeft(const PointTemplate<T> &topLeft) -> RectTemplate<T>&;
    auto setTopRight(const PointTemplate<T> &topRight) -> RectTemplate<T>&;
    auto setBottomLeft(const PointTemplate<T> &bottomLeft) -> RectTemplate<T>&;
    auto setBottomRight(const PointTemplate<T> &bottomRight) -> RectTemplate<T>&;

    auto y() const -> T;
    auto x() const -> T;

    template <typename Z>
    constexpr auto castTo() const -> RectTemplate<Z>;

    auto intersects(const RectTemplate<T> &other) const -> bool;
    auto intersected(const RectTemplate<T> &other) const -> RectTemplate<T>;

    constexpr auto contains(const PointTemplate<T> &point, T precision = T(0)) const -> bool;
    constexpr auto containsAny(const std::initializer_list<PointTemplate<T>>& points, T precision = T(0)) const -> bool;

    nd
    constexpr auto isNull() const -> bool;

    constexpr auto operator=(const RectTemplate<T> &other) -> RectTemplate<T>&;
    constexpr auto operator=(RectTemplate<T> &&other) -> RectTemplate<T>& = default;
    constexpr auto operator*(T val) const -> RectTemplate<T>;
    constexpr auto operator/(T val) const -> RectTemplate<T>;
    constexpr auto operator+(const PointTemplate<T> &other) const -> RectTemplate<T>;

    constexpr auto operator*=(double d) -> RectTemplate<T>;
    constexpr auto operator/=(double d) -> RectTemplate<T>;
    constexpr auto operator+=(const RectTemplate<T> &other) -> RectTemplate<T>;

    constexpr auto operator==(const RectTemplate &other) const -> bool;

    // TODO: move outside of class definition
    static
    auto load(const VersionFileController &versionController, ReadableAbstract &reader)
        noexcept -> WPair<Error, RectTemplate<T>>
    {
        RectTemplate<T> d;
        if (versionController.getVersionRectTemplate() != 0)
            return {Error::makeErrVersion(), {}};

        {
            auto [res, point] = PointTemplate<T>::load (versionController, reader);
            if (res)
                return {res, {}};

            d._topLeft = std::move(point);
        }

        {
            auto [res, point] = PointTemplate<T>::load (versionController, reader);
            if (res)
                return {res, {}};

            d._bottomRight = std::move(point);
        }

        return {Error::makeOk(), d};
    }


    /**
     * \return negative value in case of error
     */
    static
    auto write (WritableAbstract &writable, const RectTemplate<T> &source) -> int
    {
        if (PointTemplate<T>::write(writable, source._topLeft) < 0)
            return -1;
        if (PointTemplate<T>::write(writable, source._bottomRight) < 0)
            return -1;
        return 0;
    }
};

template<typename T>
inline RectTemplate<T> RectTemplate<T>::intersected(const RectTemplate<T> &other) const
{
    if (!intersects(other))
        return { (T) 0, (T) 0, (T) 0, (T) 0 };

    return {
        std::max(this->_topLeft.x(), other._topLeft.x()),
        std::max(this->_topLeft.y(), other._topLeft.y()),

        std::min(this->_bottomRight.x(), other._bottomRight.x()),
        std::min(this->_bottomRight.y(), other._bottomRight.y())
    };
}

/**
 * @return true iff exists some are that is shared between this and other
 * */
template<typename T>
inline bool RectTemplate<T>::intersects(const RectTemplate<T> &other) const
{
    if (this->containsAny({other.topLeft(), other.topRight(), other.bottomRight(), other.bottomLeft()}))
        return true;

    if (other.containsAny({topLeft(), topRight(), bottomRight(), bottomLeft()}))
        return true;

    return false;
}

template<typename T>
inline constexpr bool RectTemplate<T>::contains(const PointTemplate<T> &point, T precision) const
{
    return  point.x() >= _topLeft.x() - precision &&
            point.y() >= _topLeft.y() - precision &&
            point.x() <= _bottomRight.x() + precision &&
            point.y() <= _bottomRight.y() + precision;
}

template <class T>
inline constexpr auto RectTemplate<T>::containsAny(const std::initializer_list<PointTemplate<T>> &points,
                                                T precision) const -> bool
{
    for (const auto &ref: std::as_const(points)) {
        if (contains(ref, precision))
            return true;
    }

    return false;
}

template<typename T>
inline constexpr RectTemplate<T> RectTemplate<T>::operator+=(const RectTemplate<T> &other)
{
    _topLeft += other;
    _bottomRight += other;
    return *this;
}

template<typename T>
inline constexpr RectTemplate<T> RectTemplate<T>::operator/=(double d)
{
    _topLeft /= d;
    _bottomRight /= d;
    return *this;
}

template<typename T>
inline constexpr RectTemplate<T> RectTemplate<T>::operator*=(double d)
{
    _topLeft *= d;
    _bottomRight *= d;
    return *this;
}

template<typename T>
inline constexpr auto RectTemplate<T>::operator+(const PointTemplate<T> &other) const -> RectTemplate<T>
{
    return {
        _topLeft + other,
        _bottomRight + other
    };
}

template<typename T>
inline constexpr auto RectTemplate<T>::operator/(T val) const -> RectTemplate<T>
{
    return {
        _topLeft / val,
        _bottomRight / val
    };
}

template<typename T>
inline constexpr auto RectTemplate<T>::operator*(T val) const -> RectTemplate<T>
{
    return {
        _topLeft * val,
        _bottomRight * val
    };
}

template<typename T>
inline constexpr auto RectTemplate<T>::operator=(const RectTemplate<T> &other) -> RectTemplate<T>&
{
    this->_topLeft = other._topLeft;
    this->_bottomRight = other._bottomRight;

    return *this;
}

template<typename T>
inline constexpr auto RectTemplate<T>::operator==(const RectTemplate &other) const -> bool
{
    if (this == &other)
        return true;

    return  this->_bottomRight == other._bottomRight &&
            this->_topLeft == other._topLeft;
}

template<typename T>
inline constexpr auto RectTemplate<T>::left() const -> T
{
    return _topLeft.x();
}

template<typename T>
inline constexpr auto RectTemplate<T>::right() const -> T
{
    return _bottomRight.x();
}

template<typename T>
inline constexpr auto RectTemplate<T>::bottom() const -> T
{
    return _bottomRight.y();
}

template<typename T>
inline constexpr auto RectTemplate<T>::top() const -> T
{
    return _topLeft.y();
}

template<typename T>
constexpr auto RectTemplate<T>::isNull() const -> bool
{
    return top() == bottom() || left() == right();
}

template<typename T>
auto RectTemplate<T>::x() const -> T
{
    return _topLeft.x();
}

template<typename T>
auto RectTemplate<T>::y() const -> T
{
    return _topLeft.y();
}

template<typename T>
auto RectTemplate<T>::setBottomRight(const PointTemplate<T> &bottomRight) -> RectTemplate<T>&
{
    _bottomRight = bottomRight;
    return *this;
}

template<typename T>
auto RectTemplate<T>::setTopRight(const PointTemplate<T> &topRight) -> RectTemplate<T>&
{
    _topLeft.setY(topRight.y());
    _bottomRight.setX(topRight.x());
    return *this;
}

template<typename T>
auto RectTemplate<T>::setTopLeft(const PointTemplate<T> &topLeft) -> RectTemplate<T>&
{
    _topLeft = topLeft;
    return *this;
}

template<typename T>
inline auto RectTemplate<T>::setX(T x) -> RectTemplate<T>&
{
    const auto diff = _bottomRight.x() - _topLeft.x();

    W_ASSERT(diff >= 0.);

    _topLeft.setY(x);

    _bottomRight = {
            _bottomRight.x(),
            diff + x
    };

    return *this;
}

template<typename T>
inline auto RectTemplate<T>::setY(T y) -> RectTemplate<T>&
{
    const auto diff = _bottomRight.y() - _topLeft.y();

    W_ASSERT(diff >= 0.);

    _topLeft.setY(y);

    _bottomRight = {
            _bottomRight.x(),
            diff + y
    };

    return *this;
}

template<typename T>
inline constexpr RectTemplate<T> &RectTemplate<T>::right(T amount)
{
    return {
        _topLeft.right(amount),
        _bottomRight.right(amount)
    };
}

template<typename T>
inline constexpr auto RectTemplate<T>::bottom(T amount) -> RectTemplate<T>&
{
    return {
        _topLeft.bottom(amount),
        _bottomRight.bottom(amount)
    };
}

template<typename T>
inline constexpr auto RectTemplate<T>::top(T amount) -> RectTemplate<T>&
{
    return {
        _topLeft.top(amount),
        _bottomRight.top(amount)
    };
}

template<typename T>
inline void RectTemplate<T>::translate(const T &x, const T &y)
{
    _topLeft += PointTemplate<T> (x, y);
    _bottomRight += PointTemplate<T> (x, y);
}

template<typename T>
inline constexpr auto RectTemplate<T>::left(T amount) -> RectTemplate<T> &
{
    return {
        _topLeft.left(amount),
        _bottomRight.left(amount)
    };
}

template<typename T>
template<typename Z>
inline constexpr RectTemplate<Z> RectTemplate<T>::castTo() const {
    return RectTemplate<Z>(_topLeft.template castTo<Z>(), _bottomRight.template castTo<Z>());
}

template<typename T>
inline auto RectTemplate<T>::setWidth(T width) -> RectTemplate<T>&
{
    _bottomRight.setX(
        _topLeft.x() + width
    );
    return *this;
}

template<typename T>
inline auto RectTemplate<T>::setHeight(T height) -> RectTemplate<T>&
{
    _bottomRight.setY(
        _topLeft.y() + height
    );
    return *this;
}

template<typename T>
inline auto RectTemplate<T>::bottomLeft() const -> const PointTemplate<T>
{
    return PointTemplate<T> {
        _topLeft.x(),
        _bottomRight.y()
    };
}

template<typename T>
inline auto RectTemplate<T>::bottomRight() const -> const PointTemplate<T> &
{
    return _bottomRight;
}

template<typename T>
inline auto RectTemplate<T>::setBottomLeft(const PointTemplate<T> &bottomLeft) -> RectTemplate<T>&
{
    _topLeft.setX(
            bottomLeft.x()
    );

    _bottomRight.setY(
        bottomLeft.y()
    );
    return *this;
}

template<typename T>
inline auto RectTemplate<T>::topRight() const -> const PointTemplate<T>
{
    return PointTemplate<T> {
        _bottomRight.x(),
        _topLeft.y()
    };
}

template<typename T>
inline const PointTemplate<T> &RectTemplate<T>::topLeft() const
{
    return this->_topLeft;
}

template<typename T>
inline constexpr RectTemplate<T> RectTemplate<T>::addY(T y) const
{
    return RectTemplate {
        _topLeft.x(),
        _topLeft.y() + y,
        width(),
        height()
    };
}

template<typename T>
inline constexpr RectTemplate<T>::RectTemplate(const PointTemplate<T> &point, const WSizeTemplate<T> &size)
    : _topLeft(point)
    , _bottomRight(point.x() + size.getWidth(), point.y() + size.getHeight())
{
}

template<typename T>
inline constexpr RectTemplate<T>::RectTemplate(T xTopLeft, T yTopLeft, T width, T height)
    : _topLeft(xTopLeft, yTopLeft)
    , _bottomRight(xTopLeft + width, yTopLeft + height)
{
    W_ASSERT(width >= T(0));
    W_ASSERT(height >= T(0));
}

template<typename T>
inline constexpr RectTemplate<T>::RectTemplate(const WSizeTemplate<T> &size)
    : _topLeft((T) 0, (T) 0)
    , _bottomRight(size.getWidth(), size.getHeight())
{
}

template<typename T>
inline constexpr RectTemplate<T>::RectTemplate(
            const PointTemplate<T> &pt1,
            const PointTemplate<T> &pt2
        )
    : _topLeft(
        WUtils::min(
                    pt1.x(),
                    pt2.x()
            ),
        WUtils::min(
                    pt1.y(),
                    pt2.y()
            )
        )
    , _bottomRight(
                WUtils::max(
                    pt1.x(),
                    pt2.x()
            ),
                WUtils::max(
                    pt1.y(),
                    pt2.y()
            )
        )
{
}

template<typename T>
inline constexpr RectTemplate<T>::RectTemplate(const RectTemplate<T> &other)
    : _topLeft(other._topLeft)
    , _bottomRight(other._bottomRight)
{
}

template <class T>
inline auto RectTemplate<T>::height() const -> T
{
    return this->_bottomRight.y() - this->_topLeft.y();
}

template <class T>
inline auto RectTemplate<T>::width() const -> T
{
    return this->_bottomRight.x() - this->_topLeft.x();
}

template <class T>
inline std::ostream& operator<<(std::ostream& os, const RectTemplate<T>& dt)
{
    os << dt.topLeft();
    os << dt.bottomRight();
    return os;
}

using WRect = RectTemplate<int>;
