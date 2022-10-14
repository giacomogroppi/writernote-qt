#include "StrokePre.h"
#include "core/core.h"
#include "touch/dataTouch/stroke/stroke_drawer.h"

StrokePre::StrokePre() noexcept :
    Stroke(),
    WImage(1),
    _last_draw_point(nullptr),
    _last_draw_press(nullptr)
{
    W_ASSERT(is_normal());
    W_ASSERT(Stroke::isEmpty());
    W_ASSERT(not WImage::isNull());
}

void StrokePre::merge(Stroke &res)
{
    W_ASSERT(this->already_merge == false);
    W_ASSERT(_point.length() == _pressure.length());
    W_ASSERT(res.isEmpty());

    int i;

    if (not Stroke::is_normal()) {
        const auto &s = dynamic_cast<const Stroke &>(*this);
        Stroke::copy(s, res);
        return;
    }

    const auto l = _point.length();

    Stroke::preappend(l);

    for (i = 0; i < l; i++) {
        const auto *data_point = _point.get_first();
        const auto *data_press = _pressure.get_first();

        res.append(*data_point, *data_press);

        WDelete(data_point);
        WDelete(data_press);
    }

    res.setMetadata(
                    Stroke::getMetadata()
                );

#ifdef DEBUGINFO
    already_merge = true;
#endif // DEBUGINFO
}

void StrokePre::adjust(const QPointF &delta)
{
    if(likely(Stroke::is_normal())){
        for(auto &point : this->_point){
            point -= delta;
        }
    }else{
        stroke_complex_translate(this, -delta);
    }
}

void StrokePre::setAlfaColor(int alfa)
{
    W_ASSERT(alfa >= 0 and alfa <= 255);
    Stroke::setAlfaColor(alfa);
}

void StrokePre::setTime(int time)
{
    W_ASSERT( time == -1 or time >= 0 );
    Stroke::setPositioneAudio(time);
}

QRect StrokePre::getBiggerPointInStroke() const
{
    /** TODO --> define a cache */
    const auto res = Stroke::getBiggerPointInStroke(this->_point.constBegin(),
                                                    this->_point.constEnd(),
                                                    *this);
    return res;
}

QRect StrokePre::getFirstAndLast() const
{
    const auto &first = *_point.constBegin();
    const auto &last  = _point.last();

    return QRect(first.toPoint(), last.toPoint());
}

pressure_t StrokePre::getPressure() const
{
    W_ASSERT(!this->_pressure.isEmpty());
    return *this->_pressure.constBegin();
}

void StrokePre::set_complex(StrokeProp type, void *data)
{
    Stroke::set_complex(type, data);
    W_ASSERT(Stroke::is_complex());
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
        const auto &img = dynamic_cast<const QImage &>(*this);
        const auto target = img.rect();

        W_ASSERT(img.isNull() == false);

        painter.drawImage(target, img);
    }else {
        W_ASSERT(not Stroke::isEmpty());
        Stroke::draw(painter, false, 0, pen, prop);
    }
}

QColor StrokePre::getColor(double division) const
{
    return Stroke::getColor(division);
}

StrokePre &StrokePre::operator=(const StrokePre &other)
{
    Stroke::operator=(other);
    WImage::operator=(other);
    this->_point = other._point;
    this->_pressure = other._pressure;

#ifdef DEBUGINFO
    this->already_merge = other.already_merge;
#endif // DEBUGINFO

    W_ASSERT(Stroke::cmp(*this, other) == true);

    return *this;
}

void StrokePre::append(const point_s &point, const pressure_t &press, QPen &pen, cdouble prop)
{
    const auto normal = is_normal();

    if (normal) {
        QPainter painter;
        auto &img = dynamic_cast<QImage &>(*this);

        painter.begin(&img);

        W_ASSERT(img.isNull() == false);
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
                                   dynamic_cast<StrokePre &>(*this),
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
