#pragma once

#include "Stroke.h"
#include "core/WList.h"
#include "core/Pixmap/WPixmap.h"
#include "touch/object_finder/model_finder/model_finder.h"
#include "core/Pixmap/WPixmap.h"
#include "touch/dataTouch/Page/Page.h"

class StrokePre
{
private:
    WPixmap _img;
    std::unique_ptr<Stroke> _stroke;

    WList<PointF>      _point;
    WList<pressure_t>   _pressure;

    WList<PointF>       ::const_iterator   _last_draw_point;
    WList<pressure_t>   ::const_iterator   _last_draw_press;

    pressure_t _max_pressure;
    PointSettable _min, _max;

    constexpr static bool StrokePreDebug = false;

    [[nodiscard]] bool isImageEmpty() const;

#ifdef DEBUGINFO
    bool already_merge = false;
#endif // DEBUGINFO

    [[nodiscard]] WList<pressure_t>::const_iterator get_last_press() const;
    [[nodiscard]] WList<PointF>::const_iterator get_last_point() const;
    [[nodiscard]] const Stroke &get_stroke_for_draw() const;

    void setStrokeComplex(std::unique_ptr<Stroke> &&stroke);
public:
    StrokePre ();
    StrokePre (const StrokePre &other) noexcept;
    StrokePre (StrokePre &&other) noexcept;
    ~StrokePre() noexcept;

    void adjust(const PointF &delta);
    void setAlfaColor(int alfa);

    void setTime(int time);
    void setColor(const WColor &color) noexcept;
    [[nodiscard]] bool isEmpty() const noexcept;
    [[nodiscard]] RectF getBiggerPointInStroke() const;
    [[nodiscard]] RectF getFirstAndLast() const;
    [[nodiscard]] pressure_t getPressure() const;
    [[nodiscard]] const PointF &last() const;

    [[nodiscard]] auto constBegin() const { return _point.constBegin(); };
    [[nodiscard]] auto constEnd() const { return _point.constEnd(); };

    void reset();
    void reset_img();

    void draw(WPainter &painter, WPen &pen, double prop, const PointF &pointFirstPage);
    void append(const PointF &point, const pressure_t &press, WPen &pen, double prop);
    [[nodiscard]] WColor getColor(double division = 1.) const;

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

inline const PointF &StrokePre::last() const
{
    return _point.last();
}

inline void StrokePre::reset()
{
    *this = StrokePre();
}

inline void StrokePre::setColor(const WColor &color) noexcept
{
    _stroke->setColor(WColor(color));
}

inline const Stroke &StrokePre::get_stroke_for_draw() const
{
    return *_stroke;
}

inline WList<pressure_t>::const_iterator StrokePre::get_last_press() const
{
    return _last_draw_press;
}

inline WList<PointF>::const_iterator StrokePre::get_last_point() const
{
    W_ASSERT(this->_point.size() > 1);
    return _last_draw_point;
}

force_inline void StrokePre::draw(WPainter &painter, WPen &pen, double prop, const PointF& pointFirstPage)
{
    WDebug(StrokePreDebug, "Pointer" << this);

    painter.setAntialeasing();
    
    if (_stroke->isEmpty()) {
        W_ASSERT(_stroke->type() == Stroke::COMPLEX_NORMAL);

        if (this->_point.size() == 1)
            return;

        const RectF source {
                           this->_min.x() - this->_max_pressure * 5.,
                           this->_min.y() - this->_max_pressure * 5.,
                           this->_max.x() + this->_max_pressure * 5.,
                           this->_max.y() + this->_max_pressure * 5.
        };

        const RectF target {
            source.topLeft() - pointFirstPage,
            source.bottomRight() - pointFirstPage
        };

        W_ASSERT(_img.isNull() == false);

        //_img.write("/Users/giacomo/Desktop/tmp_foto/prova.png", "PNG");

        //painter.drawPixmap(target, _img, source);
        painter.drawPixmap(RectF(0, 0, Page::getWidth(), Page::getHeight()), _img, _img.rect());
    } else {
        _stroke->draw(painter, false, 0, pen, prop);
    }
}
