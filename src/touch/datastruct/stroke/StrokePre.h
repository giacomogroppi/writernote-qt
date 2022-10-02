#pragma once

#include "Stroke.h"
#include "core/WList.h"
#include "core/WImage.h"

class StrokePre: private Stroke, private WImage {
private:
    WList<point_s>      _point;
    WList<pressure_t>   _pressure;
public:
    StrokePre() noexcept;
    ~StrokePre() = default;

    void setColor(const QColor &color) noexcept override;
    [[nodiscard]] bool isEmpty() const noexcept;
    [[nodiscard]] int length() const;
    void merge(Stroke &to) const noexcept;
    void reset() noexcept;
};

inline bool StrokePre::isEmpty() const noexcept
{
    return this->_point.isEmpty();
}

inline int StrokePre::length() const
{
    W_ASSERT(Stroke::is_normal());
    return _point.length();
}

inline void StrokePre::setColor(const QColor &color) noexcept
{
    Stroke::setColor(color);
}
