//
// Created by Giacomo Groppi on 08/05/23.
//

#ifndef WRITERNOTE_WSIZETEMPLATE_H
#define WRITERNOTE_WSIZETEMPLATE_H


template <typename T>
class WSizeTemplate {
private:
    T _width;
    T _height;
public:
    constexpr WSizeTemplate(T width, T height) noexcept;
    constexpr WSizeTemplate() noexcept;

    constexpr T getWidth() const noexcept;
    constexpr T getHeight() const noexcept;

    constexpr T setWidth(T width) noexcept;
    constexpr T setHeight(T height) noexcept;

    template <typename Z>
    constexpr WSizeTemplate<Z> castTo() const noexcept;

    constexpr bool operator==(const WSizeTemplate<T> &other) const noexcept;
    constexpr WSizeTemplate<T> &operator=(const WSizeTemplate<T> &other) noexcept;
    constexpr WSizeTemplate<T> &operator*=(double delta) noexcept;
    constexpr WSizeTemplate<T> operator*(double val) noexcept;
};

template<typename T>
template<typename Z>
inline constexpr WSizeTemplate<Z> WSizeTemplate<T>::castTo() const noexcept
{
    return {
        static_cast<Z>(_width),
        static_cast<Z>(_height)
    };
}

template<typename T>
inline constexpr WSizeTemplate<T> WSizeTemplate<T>::operator*(double val) noexcept
{
    WSizeTemplate<T> res;
    res._height = this->_height * val;
    res._width = this->_width * val;
    return res;
}

template<typename T>
inline constexpr WSizeTemplate<T> &WSizeTemplate<T>::operator*=(double delta) noexcept
{
    _width *= delta;
    _height *= delta;
    return *this;
}

template<typename T>
inline constexpr WSizeTemplate<T> &WSizeTemplate<T>::operator=(const WSizeTemplate<T> &other) noexcept
{
    _width = other._width;
    _height = other._height;
    return *this;
}

template<typename T>
inline constexpr bool WSizeTemplate<T>::operator==(const WSizeTemplate<T> &other) const noexcept
{
    return this->_height == other._height && this->_width == other._width;
}

template<typename T>
inline constexpr T WSizeTemplate<T>::setHeight(T height) noexcept
{
    _height = height;
}

template<typename T>
inline constexpr T WSizeTemplate<T>::setWidth(T width) noexcept
{
    _width = width;
}

template<typename T>
inline constexpr T WSizeTemplate<T>::getWidth() const noexcept
{
    return this->_width;
}

template<typename T>
inline constexpr T WSizeTemplate<T>::getHeight() const noexcept
{
    return this->_height;
}

template<typename T>
inline constexpr WSizeTemplate<T>::WSizeTemplate() noexcept
    : _width((T) 0)
    , _height((T) 0 )
{

}

template<typename T>
inline constexpr WSizeTemplate<T>::WSizeTemplate(T width, T height) noexcept
    : _width(width)
    , _height(height)
{

}

using WSizeF = WSizeTemplate<double>;
using WSize = WSizeTemplate<int>;

#endif //WRITERNOTE_WSIZETEMPLATE_H
