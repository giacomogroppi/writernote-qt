#pragma once

#include "Stroke.h"
#include "core/WList.h"
#include "core/WImage.h"

class StrokePre: private Stroke, private WImage {
private:
    WList<point_s>      _point;
    WList<pressure_t>   _pressure;
#ifdef DEBUGINFO
    bool already_merge = false;
#endif // DEBUGINFO
public:
    StrokePre() noexcept;
    ~StrokePre();

    void setColor(const QColor &color) noexcept;
    [[nodiscard]] bool isEmpty() const noexcept;
    [[nodiscard]] int length() const;
    [[nodiscard]] bool is_normal() const;
    void reset() noexcept;

    Stroke& merge();
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

inline bool StrokePre::is_normal() const
{
    return Stroke::is_normal();
}

