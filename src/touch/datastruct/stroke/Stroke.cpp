#include "Stroke.h"
#include "stroke_drawer.h"
#include "touch/object_finder/model/model.h"
#include <QDebug>
#include <QPainter>
#include "touch/datastruct/stroke_file.h"

Stroke::Stroke()
{
    reset();
}

Stroke::Stroke(const Stroke &data)
{
    reset();
    *this = data;
}

void Stroke::__setPressureFirstPoint(const pressure_t pres)
{
    if(_pressure.length())
        _pressure.operator[](0) = pres;
    else
        _pressure.append(pres);
}

int Stroke::save(WZipWriterSingle &file) const
{
    return stroke_file::save(*this, file);
}

int Stroke::load(WZipReaderSingle &reader, int version)
{
    return stroke_file::load(*this, version, reader);
}

void Stroke::setMetadata(cint posizione_audio, const colore_s &color)
{
    _metadata.posizione_audio = posizione_audio;
    memcpy(&_metadata.color, &color, sizeof(color));
}

size_t Stroke::createControll() const
{
    size_t controll = 0;
    int i, len;

    if(unlikely(this->is_complex())){
        return 0;
    }

    len = length();

    for(i = 0; i < NCOLOR; i++){
        controll += _metadata.color.colore[i];
    }

    for(i = 0; i < len; i++){
        const point_s &point = _point.at(i);
        controll += diff(point.x());
        controll += diff(point.y());
    }

    len = _pressure.length();
    for(i = 0; i < len; i++){
        controll += diff(_pressure.at(i));
    }

    return controll;
}

size_t Stroke::getSizeInMemory() const
{
    if(unlikely(this->is_complex()))
        return 0;

    return sizeof(point_s) * length();
}

size_t Stroke::getSizeInFile() const
{
    return stroke_file::get_size_in_file(*this);
}

void Stroke::decreasePrecision()
{
    int i, len;

    if(unlikely(is_complex()))
        return;

    len = length();

    for(i = 1; i < len - 1; i++){
        if(i % 2 == 0){
            this->removeAt(i);
        }
    }
}

void Stroke::movePoint(const QPointF &translation)
{
    int i, len;

    if(unlikely(is_complex())){
        stroke_complex_translate(this, translation);
        return;
    }

    len = this->length();

    for(i = 0; i < len; i++){
        point_s &point = at_mod(i);
        point += translation;
    }
}

void Stroke::reset()
{
    _flag = UPDATE_BIGGER_DATA | UPDATE_PRESSURE;

    /*
     * as we do not know the type of stroke
     * it will become, they week it as normal
     */

    if(likely(is_normal())){
        W_ASSERT(!_complex);
    }else{
        W_ASSERT(_complex);
        WFree(_complex);
        _complex = NULL;
    }

    W_ASSERT(!_complex);

    _prop = COMPLEX_NORMAL;
    _pressure.clear();
    _point.clear();
    _metadata = {
            .posizione_audio = -1,
            .color = colore_s()
    };

    W_ASSERT(this->is_normal());
    W_ASSERT(!_complex);
}

bool Stroke::cmp(const Stroke &stroke1, const Stroke &stroke2)
{
    int i, len;

    if(likely(stroke1.is_normal())){
        if(unlikely(!stroke2.is_normal()))
            return false;

        len = stroke1.length();
        if(len != stroke2.length())
            return false;

        for (i = 0; i < len; i ++){
            const point_s &point1 = stroke1.at(i);
            const point_s &point2 = stroke2.at(i);
            if(memcmp(&point1, &point2, sizeof(point1)) != 0){
                return false;
            }
        }
    }else{
        return stroke_complex_cmp(&stroke1, &stroke2);
    }

    return true;
}

const point_s &Stroke::last() const
{
    return _point.last();
}

void Stroke::draw(
        QPainter    &painter,
        cbool       is_rubber,
        cint        page,
        QPen        &pen,
        cdouble     prop) const
{
    stroke_drawer::draw_stroke(painter, *this, page, pen, is_rubber, prop);
}
