#include "StrokePre.h"
#include "core/core.h"
#include "touch/dataTouch/stroke/stroke_drawer.h"

StrokePre::StrokePre() noexcept :
    Stroke(),
    WImage(1),
    _last_draw_point(nullptr),
    _last_draw_press(nullptr)
{
}

Stroke& StrokePre::merge()
{
    W_ASSERT(this->already_merge == false);
    W_ASSERT(_point.length() == _pressure.length());

    int i;

    if(!Stroke::isEmpty()){
        W_ASSERT(Stroke::is_complex());
        return *this;
    }

    const auto l = _point.length();

    Stroke &res = *this;

    for(i = 0; i < l; i++){
        const auto *data_point = _point.get_first();
        const auto *data_press = _pressure.get_first();

        Stroke::append(*data_point, *data_press);

        delete data_point;
        delete data_press;
    }

#ifdef DEBUGINFO
    already_merge = true;
#endif // DEBUGINFO
    return *this;
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
    const auto &first = _point.constBegin();
    const auto &last  = _point.constEnd();

    return QRect(   first-> toPoint(),
                    last->  toPoint());
}

void StrokePre::draw(QPainter &painter)
{
    const auto &img = dynamic_cast<const QImage &>(*this);
    const auto target = img.rect();

    W_ASSERT(img.isNull() == false);

    painter.drawImage(target, img);
}

QColor StrokePre::getColor(double division) const
{
    return Stroke::getColor(division);
}

StrokePre &StrokePre::operator=(const StrokePre &other)
{
    Stroke::operator=(other);
    this->_point = other._point;
    this->_pressure = other._pressure;

#ifdef DEBUGINFO
    this->already_merge = other.already_merge;
#endif // DEBUGINFO

    return *this;
}

void StrokePre::append(const point_s &point, const pressure_t &press, QPen &pen, cdouble prop)
{
    QPainter painter;
    auto &img = dynamic_cast<QImage &>(*this);
    const auto is_normal = Stroke::is_normal();

    W_ASSERT(img.isNull() == false);

    painter.begin(&img);

    if (unlikely(!is_normal)){
        W_ASSERT(_point.isEmpty());
        W_ASSERT(_pressure.isEmpty());
        auto *s = dynamic_cast<Stroke *>(this);

        stroke_complex_append(s, point.toQPointF(1.));
    } else {
        _point.append(point);
        _pressure.append(press);
    }

    core::painter_set_antialiasing(painter);
    core::painter_set_source_over(painter);

    if(is_normal and unlikely(_point.length() == 1)){
        _last_draw_point = this->_point.constBegin();
        _last_draw_press = this->_pressure.constBegin();
        goto out;
    }

    stroke_drawer::draw_stroke(painter,
                               dynamic_cast<StrokePre &>(*this),
                               pen, prop);

    if(is_normal){
        this->_last_draw_press ++;
        this->_last_draw_point ++;
    }

out:
    painter.end();
}
