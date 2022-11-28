#include "StrokePre.h"
#include "core/core.h"
#include "touch/dataTouch/stroke/stroke_drawer.h"
#include "touch/dataTouch/stroke/StrokeNormal.h"

StrokePre::StrokePre() noexcept :
    _img(1),
    _stroke(),
    _last_draw_point(nullptr),
    _last_draw_press(nullptr)
{
    _stroke = new StrokeNormal();

    W_ASSERT(_stroke->isEmpty());
    W_ASSERT(not _img.isNull());
}

StrokePre::~StrokePre()
{
    if(this->_stroke)
        delete this->_stroke;
}

Stroke *StrokePre::merge()
{
    using namespace WCommonScript;
    W_ASSERT(this->already_merge == false);
    W_ASSERT(_point.length() == _pressure.length());
    W_ASSERT(this->_stroke != nullptr);

    int i;

#ifdef DEBUGINFO
    already_merge = true;
#endif // DEBUGINFO

    if (this->_stroke->type() != Stroke::COMPLEX_NORMAL) {
        W_ASSERT(this->_point.isEmpty());
        auto *res = this->_stroke;
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

    return this->_stroke;
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
    const auto res = StrokeNormal::getBiggerPointInStroke(
                            this->_point.constBegin(),
                            this->_point.constEnd(),
                            this->_stroke);
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

void StrokePre::set_complex(StrokeProp type, void *data)
{
    _stroke.set_complex(type, data);
    W_ASSERT(_stroke.is_complex());
    WDebug(StrokePreDebug, "Pointer" << this);
}

void StrokePre::reset_img()
{
    auto &img = dynamic_cast<WImage &>(*this);
    img = WImage();
}

void StrokePre::draw(QPainter &painter, QPen &pen, double prop)
{
    WDebug(StrokePreDebug, "Pointer" << this);
    if (is_normal()) {
        const auto target = _img.rect();

        W_ASSERT(_img.isNull() == false);

        painter.drawImage(target, _img);
    }else {
        W_ASSERT(not _stroke.isEmpty());
        _stroke.draw(painter, false, 0, pen, prop);
    }
}

QColor StrokePre::getColor(double division) const
{
    return _stroke.getColor(division);
}

StrokePre &StrokePre::operator=(const StrokePre &other)
{
    _stroke.operator=(other._stroke);
    _img.operator=(other._img);
    this->_point = other._point;
    this->_pressure = other._pressure;

#ifdef DEBUGINFO
    this->already_merge = other.already_merge;
#endif // DEBUGINFO

    W_ASSERT(Stroke::cmp(this->_stroke, other._stroke) == true);

    return *this;
}

void StrokePre::append(const point_s &point, const pressure_t &press, QPen &pen, cdouble prop)
{
    const auto normal = is_normal();

    if (normal) {
        QPainter painter;

        painter.begin(&this->_img);

        W_ASSERT(this->_img.isNull() == false);
        _point.append(point);
        _pressure.append(press);

        core::painter_set_antialiasing(painter);
        core::painter_set_source_over(painter);

        if(unlikely(_point.length() == 1)){
            _last_draw_point = this->_point.constBegin();
            _last_draw_press = this->_pressure.constBegin();
            goto out;
        }

        stroke_drawer::draw_stroke(painter,
                                   *this,
                                   pen, prop);


        this->_last_draw_press ++;
        this->_last_draw_point ++;


out:
        painter.end();

    } else {
        W_ASSERT(_point.isEmpty());
        W_ASSERT(_pressure.isEmpty());
        auto *s = dynamic_cast<Stroke *>(this);

        stroke_complex_append(s, point.toQPointF(1.));
    }
}
