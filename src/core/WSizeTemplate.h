//
// Created by Giacomo Groppi on 08/05/23.
//

#ifndef WRITERNOTE_WSIZETEMPLATE_H
#define WRITERNOTE_WSIZETEMPLATE_H


template <typename T>
class WSizeTemplate {
public:
    constexpr explicit WSizeTemplate(T width, T height);
    constexpr explicit WSizeTemplate();

    T getWidth() const;
    T getHeight() const;

    T setWidth(T width);
    T setHeight(T height);

    template <typename Z>
    constexpr WSizeTemplate<Z> castTo() const;

    bool operator==(const WSizeTemplate<T> &other) const;
    WSizeTemplate<T> operator=(const WSizeTemplate<T> &other);
    WSizeTemplate<T> operator*=(const double delta);
    WSizeTemplate<T> operator*(double val);
};

using WSizeF = WSizeTemplate<double>;
using WSize = WSizeTemplate<int>;

#endif //WRITERNOTE_WSIZETEMPLATE_H
