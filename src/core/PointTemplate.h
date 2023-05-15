//
// Created by Giacomo Groppi on 10/05/23.
//

#ifndef WRITERNOTE_POINTTEMPLATE_H
#define WRITERNOTE_POINTTEMPLATE_H

template <typename T>
class PointTemplate {
private:
    T _x, _y;
public:
    constexpr PointTemplate() = default;
    constexpr PointTemplate(T x, T y);

    ~PointTemplate() = default;

    constexpr T x() const;
    constexpr T y() const;

    T &rx();
    T &ry();

    T setX(T x);
    T setY(T y);

    template <typename Z>
    PointTemplate<Z> castTo() const;

    PointTemplate<T>& operator=(const PointTemplate<T> &other);
    bool operator==(const PointTemplate<T> &other) const;

    constexpr PointTemplate<T> operator+(const PointTemplate<T> &other) const;
    constexpr PointTemplate<T> operator*(double d) const;
    constexpr PointTemplate<T> operator/(double d) const;
    constexpr PointTemplate<T> operator-(const PointTemplate<T> &other) const;

    constexpr PointTemplate<T> &operator*=(double d);
    constexpr PointTemplate<T> &operator+=(const PointTemplate<T>& d);
};

template<typename T>
inline constexpr PointTemplate<T>::PointTemplate(T x, T y)
    : _x(x)
    , _y(y)
{
}

#endif //WRITERNOTE_POINTTEMPLATE_H
