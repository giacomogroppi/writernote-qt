#pragma once

#include "Stroke.h"
#include "core/WList.h"
#include "core/WImage.h"

class StrokePre{
private:
    WImage _img;
    Stroke *_stroke;

    WList<point_s>      _point;
    WList<pressure_t>   _pressure;

    WList<point_s>      ::const_iterator   _last_draw_point;
    WList<pressure_t>   ::const_iterator   _last_draw_press;

    constexpr static bool StrokePreDebug = false;

#ifdef DEBUGINFO
    bool already_merge = false;
#endif // DEBUGINFO

    [[nodiscard]] WList<pressure_t>::const_iterator get_last_press() const;
    [[nodiscard]] WList<point_s>::const_iterator get_last_point() const;
    [[nodiscard]] const Stroke &get_stroke_for_draw() const;
public:
    StrokePre() noexcept;
    ~StrokePre();

    void adjust(const QPointF &delta);
    void setAlfaColor(int alfa);

    void setTime(int time);
    void setColor(const QColor &color) noexcept;
    [[nodiscard]] bool isEmpty() const noexcept;
    [[nodiscard]] QRect getBiggerPointInStroke() const;
    [[nodiscard]] QRect getFirstAndLast() const;
    [[nodiscard]] pressure_t getPressure() const;
    [[nodiscard]] const point_s &last() const;

    [[nodiscard]] auto constBegin() const { return _point.constBegin(); };
    [[nodiscard]] auto constEnd() const { return _point.constEnd(); };

    void reset();
    void reset_img();

    void draw(QPainter &painter, QPen &pen, double prop);
    void append(const point_s &point, const pressure_t &press, QPen &pen, cdouble prop);
    [[nodiscard]] QColor getColor(double division = 1.) const;

    [[nodiscard]] Stroke *merge();

    StrokePre &operator=(const StrokePre &other);

    friend class StrokeLineGenerator;
    friend class StrokeRectGenerator;
    friend class StrokeCircleGenerator;
    friend class stroke_drawer;
};

inline bool StrokePre::isEmpty() const noexcept
{
    return this->_point.isEmpty() and _stroke->isEmpty();
}

inline const point_s &StrokePre::last() const
{
    return _point.last();
}

inline void StrokePre::reset()
{
    *this = StrokePre();
}

inline void StrokePre::setColor(const QColor &color) noexcept
{
    _stroke->setColor(colore_s(color));
}

inline const Stroke &StrokePre::get_stroke_for_draw() const
{
    return *_stroke;
}

inline WList<pressure_t>::const_iterator StrokePre::get_last_press() const
{
    return _last_draw_press;
}

inline WList<point_s>::const_iterator StrokePre::get_last_point() const
{
    W_ASSERT(this->_point.length() > 1);
    return _last_draw_point;
}


