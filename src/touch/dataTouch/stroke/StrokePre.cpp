#include "StrokePre.h"
#include "core/core.h"
#include "touch/dataTouch/stroke/stroke_drawer.h"
#include "touch/dataTouch/stroke/StrokeNormal.h"
#include "touch/dataTouch/page/Page.h"

StrokePre::StrokePre() noexcept :
    _img(1),
    _stroke(),
    _last_draw_point(nullptr),
    _last_draw_press(nullptr)
{
    _stroke = std::shared_ptr<StrokeNormal>(new StrokeNormal);

    W_ASSERT(_stroke->isEmpty());
    W_ASSERT(not _img.isNull());
}

StrokePre::~StrokePre()
{
}

std::shared_ptr<Stroke> StrokePre::merge()
{
    using namespace WCommonScript;
    W_ASSERT(this->already_merge == false);
    W_ASSERT(_point.length() == _pressure.length());
    W_ASSERT(this->_stroke != nullptr);

#ifdef DEBUGINFO
    already_merge = true;
#endif // DEBUGINFO

    if (this->_stroke->type() != Stroke::COMPLEX_NORMAL) {
        W_ASSERT(this->_point.isEmpty());
        std::shared_ptr<Stroke> res = this->_stroke;
        this->_stroke = nullptr;
        return res;
    }

    W_ASSERT(_stroke->isEmpty());

    _stroke->preappend(_point.length());

    while (not _point.isEmpty()) {
        const auto *data_point = _point.get_first();
        const auto *data_press = _pressure.get_first();

        _stroke->append(*data_point, *data_press);

        WDelete(data_point);
        WDelete(data_press);
    }

    {
        std::shared_ptr<Stroke> res = this->_stroke;
        this->_stroke = nullptr;
        return res;
    }
}

void StrokePre::adjust(const QPointF &delta)
{
    _stroke->scale(-delta);
}

void StrokePre::setAlfaColor(int alfa)
{
    W_ASSERT(alfa >= 0 and alfa <= 255);
    _stroke->setAlfaColor(alfa);
}

void StrokePre::setTime(int time)
{
    W_ASSERT( time == -1 or time >= 0 );
    _stroke->setPositioneAudio(time);
}

QRect StrokePre::getBiggerPointInStroke() const
{
    /** TODO --> define a cache */
    const auto res = (_stroke->isEmpty()) ?
                    StrokeNormal::getBiggerPointInStroke(
                                                this->_point.constBegin(),
                                                this->_point.constEnd()) :
                    _stroke->getBiggerPointInStroke();

    return res;
}

QRect StrokePre::getFirstAndLast() const
{
    const auto &first = *_point.constBegin();
    const auto &last  = _point.last();

    return {
            first.toPoint(),
            last.toPoint()
    };
}

pressure_t StrokePre::getPressure() const
{
    W_ASSERT(!this->_pressure.isEmpty());
    return *this->_pressure.constBegin();
}

void StrokePre::reset_img()
{
    _img = WImage();
}

void StrokePre::draw(QPainter &painter, QPen &pen, double prop)
{
    WDebug(StrokePreDebug, "Pointer" << this);

    if (_stroke->isEmpty()) {
        W_ASSERT(_stroke->type() == Stroke::COMPLEX_NORMAL);

        const auto target = _img.rect();

        W_ASSERT(_img.isNull() == false);

        painter.drawImage(target, _img);
    }else {
        _stroke->draw(painter, false, 0, pen, prop);
    }
}

QColor StrokePre::getColor(double division) const
{
    return _stroke->getColor(division);
}

StrokePre &StrokePre::operator=(const StrokePre &other)
{
    if (this == &other)
        return *this;

    _stroke = other._stroke->clone();

    _img = other._img;
    this->_point = other._point;
    this->_pressure = other._pressure;

#ifdef DEBUGINFO
    this->already_merge = other.already_merge;
#endif // DEBUGINFO

    W_ASSERT(*_stroke == *other._stroke);

    return *this;
}

void StrokePre::append(const point_s &point, const pressure_t &press, QPen &pen, cdouble prop)
{
    const auto normal = (_stroke->type() == Stroke::COMPLEX_NORMAL);

    if (normal) {
        QPainter painter;

        painter.begin(&this->_img);

        W_ASSERT(this->_img.isNull() == false);

        _point.append(point);
        _pressure.append(press);

        core::painter_set_antialiasing(painter);
        core::painter_set_source_over(painter);

        if (un(_point.length() == 1)) {
            _last_draw_point = this->_point.constBegin();
            _last_draw_press = this->_pressure.constBegin();
        } else {
            StrokeNormal::drawData<WList<point_s>::const_iterator,
                    WList<pressure_t>::const_iterator> data = {
                .begin_point = this->get_last_point(),
                .end_point   = this->_point.constEnd(),
                .begin_press = this->get_last_press(),
                .end_press   = this->_pressure.constEnd(),
                .press_null  = false
            };

            StrokeNormal::draw(painter, false, 0, pen, prop, this->_stroke->getColor(1.), data);

            this->_last_draw_press ++;
            this->_last_draw_point ++;
        }

        painter.end();

    } else {
        W_ASSERT(_point.isEmpty());
        W_ASSERT(_pressure.isEmpty());

        _stroke->append(point.toQPointF(1.), press);
    }
}
