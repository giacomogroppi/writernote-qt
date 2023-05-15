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

    T x() const;
    T y() const;

    T &rx();
    T &ry();

    T setX(T x);
    T setY(T y);

    template <typename Z>
    PointTemplate<Z> castTo() const;

    PointTemplate<T>& operator=(const PointTemplate<T> &other);
    bool operator==(const PointTemplate<T> &other) const;

    PointTemplate<T> operator+(const PointTemplate<T> &other) const;
    PointTemplate<T> operator*(double d) const;
    PointTemplate<T> operator/(double d) const;
    PointTemplate<T> operator-(const PointTemplate<T> &other) const;

    PointTemplate<T> &operator*=(double d);
    PointTemplate<T> &operator+=(const PointTemplate<T>& d);
};

#endif //WRITERNOTE_POINTTEMPLATE_H
