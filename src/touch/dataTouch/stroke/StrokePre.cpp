#include "StrokePre.h"

StrokePre::StrokePre() noexcept :
    Stroke(),
    WImage()
{
    WImage::initAsPage();
}

void StrokePre::reset() noexcept
{
    Stroke::reset();
    WImage::initAsPage();
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

void StrokePre::draw(QPainter &painter, QPen &pen, cdouble prop)
{
    W_ASSERT(0);
    Stroke::draw(painter, false, 0, pen, prop);
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
