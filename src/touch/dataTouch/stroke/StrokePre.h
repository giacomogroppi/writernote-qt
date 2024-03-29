#pragma once

#include "Stroke.h"
#include "core/WList.h"
#include "core/Pixmap/WPixmap.h"
#include "touch/object_finder/model_finder/model_finder.h"
#include "core/Pixmap/WPixmap.h"
#include "touch/dataTouch/page/Page.h"
#include "Scheduler/WTimer.h"
#include "core/Settable.h"


class StrokePre final: public WObject
{
private:
    static constexpr auto _timerTime = 500;
    Fn<void()> _timerEndLambda = [this]() { this->timerEnd(); };

    WPixmap _img;
    UniquePtr<Stroke> _stroke;

    static constexpr bool timerIsSingleShot = true;
    WTimer *_timer;
    void timerEnd();
    Settable<PointF> _timerPoint;

    Fn<void()> _callUpdate;

    WListFast<PointF>       _point;
    WVector<pressure_t>     _pressure;

    WListFast<PointF>       ::const_iterator   _last_draw_point;
    WVector<pressure_t>     ::const_iterator   _last_draw_press;

    pressure_t _max_pressure;
    PointSettable _min, _max;

    constexpr static bool StrokePreDebug = false;

    nd auto isImageEmpty() const -> bool;

#ifdef DEBUGINFO
    bool already_merge = false;
#endif // DEBUGINFO

    nd auto get_last_press() const -> WVector<pressure_t>::const_iterator;
    nd auto get_last_point() const -> WListFast<PointF>::const_iterator;
    nd auto get_stroke_for_draw() const -> const Stroke &;
public:
    explicit StrokePre (Fn<void()> callUpdate);
    StrokePre (const StrokePre &other) noexcept;
    StrokePre (StrokePre &&other) noexcept;
    ~StrokePre() noexcept final;

    void adjust(const PointF &delta);
    void setAlfaColor(int alfa);

    void setTime(AudioPosition time);
    void setColor(const WColor &color) noexcept;
    nd auto isEmpty() const noexcept -> bool;
    nd auto getBiggerPointInStroke() const -> RectF;
    nd auto getFirstAndLast() const -> RectF;
    nd auto getPressure() const -> pressure_t;
    nd const PointF &last() const;

    nd auto constBegin() const { return _point.constBegin(); };
    nd auto constEnd() const { return _point.constEnd(); };

    void reset();
    void reset_img();

    void draw(WPainter &painter, double prop, const PointF &pointFirstPage);
    void append(const PointF &point, const pressure_t &press, double prop);
    nd auto getColor(double division = 1.) const -> WColor;

    auto merge() -> UniquePtr<Stroke>;

    auto operator=(const StrokePre &other) -> StrokePre&;

    friend class stroke_drawer;

    void timerReset(const PointF &point) noexcept;
};

inline bool StrokePre::isEmpty() const noexcept
{
    const auto pointsEmpty = _point.isEmpty();
    const auto strokeEmpty = _stroke->isEmpty();
    return pointsEmpty and strokeEmpty;
}

inline const PointF &StrokePre::last() const
{
    return _point.last();
}

inline void StrokePre::reset()
{
    // adjust
    *this = StrokePre(_callUpdate);
}

inline void StrokePre::setColor(const WColor &color) noexcept
{
    _stroke->setColor(WColor(color));
}

inline const Stroke &StrokePre::get_stroke_for_draw() const
{
    return *_stroke;
}

inline auto StrokePre::get_last_press() const -> WVector<pressure_t>::const_iterator
{
    return _last_draw_press;
}

inline auto StrokePre::get_last_point() const -> WListFast<PointF>::const_iterator
{
    W_ASSERT(this->_point.size() > 1);
    return _last_draw_point;
}

force_inline void StrokePre::draw(WPainter &painter, double prop, const PointF& pointFirstPage)
{
    WDebug(StrokePreDebug, "Pointer" << this);

    painter.setAntialiasing();
    
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
        painter.drawPixmap(RectF(0, 0, Page::getWidth(), Page::getHeight()).castTo<double>(), _img, _img.rect().castTo<double>());
    } else {
        _stroke->draw(painter, false, 0, prop);
    }
}
