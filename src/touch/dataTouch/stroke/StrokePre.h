#pragma once

#include "Stroke.h"
#include "core/WList.h"
#include "core/WPixmap.h"
#include "touch/object_finder/model_finder/model_finder.h"
#include "core/WPixmap.h"

class StrokePre{
private:
    WPixmap _img;
    std::shared_ptr<Stroke> _stroke;

    WList<Point>      _point;
    WList<pressure_t>   _pressure;

    WList<Point>        ::const_iterator   _last_draw_point;
    WList<pressure_t>   ::const_iterator   _last_draw_press;

    double _max_pressure;
    PointSettable _min, _max;

    constexpr static bool StrokePreDebug = false;

    bool isImageEmpty() const;

#ifdef DEBUGINFO
    bool already_merge = false;
#endif // DEBUGINFO

    [[nodiscard]] WList<pressure_t>::const_iterator get_last_press() const;
    [[nodiscard]] WList<Point>::const_iterator get_last_point() const;
    [[nodiscard]] const Stroke &get_stroke_for_draw() const;

    void setStrokeComplex(std::shared_ptr<Stroke> stroke);
public:
    StrokePre();
    ~StrokePre();

    void adjust(const PointF &delta);
    void setAlfaColor(int alfa);

    void setTime(int time);
    void setColor(const colore_s &color) noexcept;
    [[nodiscard]] bool isEmpty() const noexcept;
    [[nodiscard]] Rect getBiggerPointInStroke() const;
    [[nodiscard]] Rect getFirstAndLast() const;
    [[nodiscard]] pressure_t getPressure() const;
    [[nodiscard]] const Point &last() const;

    [[nodiscard]] auto constBegin() const { return _point.constBegin(); };
    [[nodiscard]] auto constEnd() const { return _point.constEnd(); };

    void reset();
    void reset_img();

    void draw(WPainter &painter, WPen &pen, double prop, const PointF &pointFirstPage);
    void append(const Point &point, const pressure_t &press, WPen &pen, double prop);
    [[nodiscard]] colore_s getColor(double division = 1.) const;

    std::shared_ptr<Stroke> merge();

    StrokePre &operator=(const StrokePre &other);

    friend bool model::find(StrokePre &stroke);

    friend class StrokeLineGenerator;
    friend class StrokeRectGenerator;
    friend class StrokeCircleGenerator;
    friend class stroke_drawer;
};

inline bool StrokePre::isEmpty() const noexcept
{
    return this->_point.isEmpty() and _stroke->isEmpty();
}

inline const Point &StrokePre::last() const
{
    return _point.last();
}

inline void StrokePre::reset()
{
    *this = StrokePre();
}

inline void StrokePre::setColor(const colore_s &color) noexcept
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

inline WList<Point>::const_iterator StrokePre::get_last_point() const
{
    W_ASSERT(this->_point.length() > 1);
    return _last_draw_point;
}

force_inline void StrokePre::draw(WPainter &painter, WPen &pen, double prop, const PointF& pointFirstPage)
{
    WDebug(StrokePreDebug, "Pointer" << this);

    if (_stroke->isEmpty()) {
        W_ASSERT(_stroke->type() == Stroke::COMPLEX_NORMAL);
        //W_ASSERT(painter.renderHints() & WPainter::Antialiasing);

        if (this->_point.length() == 1)
            return;

        const RectF source {
                           this->_min.x() - this->_max_pressure*5.,
                           this->_min.y() - this->_max_pressure*5.,
                           this->_max.x() + this->_max_pressure*5.,
                           this->_max.y() + this->_max_pressure*5.
        };

        const RectF target {
            source.topLeft() - pointFirstPage,
            source.bottomRight() - pointFirstPage
        };

        W_ASSERT(_img.isNull() == false);

        //_img.save("/Users/giacomo/Desktop/tmp_foto/prova.png", "PNG");

        painter.drawPixmap(target,
                           _img,
                           source);
    } else {
        _stroke->draw(painter, false, 0, pen, prop);
    }
}
