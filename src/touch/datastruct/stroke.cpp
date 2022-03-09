#include "stroke.h"
#include "page.h"
#include "utils/common_error_definition.h"
#include "dataread/xmlstruct.h"
#include "datawrite/source_read_ext.h"
#include "currenttitle/document.h"
#include "touch/tabletcanvas.h"
#include "touch/datastruct/stroke_drawer.h"
#include <QDebug>
#include <QPainter>

stroke::stroke()
{
    reset();
}

stroke::stroke(const stroke &data)
{
    reset();
    *this = data;
}

void stroke::__setPressureFirstPoint(const pressure_t pres)
{
    if(_pressure.length())
        _pressure.operator[](0) = pres;
    else
        _pressure.append(pres);
}

int stroke::save(zip_source_t *file) const
{
    int i, len;
    len = this->_pressure.length();
    // TODO
    return OK;
    cint len_point = this->length();

    SOURCE_WRITE_RETURN(file, &len_point, sizeof(len_point));
    SOURCE_WRITE_RETURN(file, &_metadata, sizeof(_metadata));

    SOURCE_WRITE_RETURN(file, &len, sizeof(len));

    for(i = 0; i < len; i++){
        SOURCE_WRITE_RETURN(file, &_pressure.at(i), sizeof(float));
    }

    for(i = 0; i < len_point; i ++){
        SOURCE_WRITE_RETURN(file, &at(i), sizeof(point_s));
    }

    return OK;
}

int stroke::load(zip_file_t *file, int version)
{
    int i, len_point;
    point_s point_append;

    // TODO
    return OK;

#ifdef ALL_VERSION
    bool page_point = false;

    struct point_s_ver_0_ver_1{
        double _x, _y;
        float pressure;
    };

    struct old_metadata_0{
        int page;
        int idtratto = -1;
        int posizione_audio;
        struct colore_s color;
    }   meta;
#endif

    this->reset();

    if(version > CURRENT_VERSION_STROKE)
        return ERROR_VERSION_NEW;

    SOURCE_READ_RETURN(file, &len_point, sizeof(len_point));

    if(version == 0){
#ifdef ALL_VERSION
        SOURCE_READ_RETURN(file, &meta, sizeof(meta));
        memcpy(&_metadata.color, &meta.color, sizeof(_metadata.color));
        memcpy(&_metadata.posizione_audio, &meta.posizione_audio, sizeof(_metadata.posizione_audio));

        // we don't load sheet from differente version
        if(unlikely(meta.idtratto < 0)){
            page_point = 1;
        }
#else
        return ERROR;
#endif
    }else{
        SOURCE_READ_RETURN(file, &_metadata, sizeof(_metadata));
    }


    if(version == 2){
        // version 2 load pressure
        int len_pressure;
        SOURCE_READ_RETURN(file, &len_pressure, sizeof(len_pressure));

        for(i = 0; i < len_pressure; i++){
            float tmp;
            SOURCE_READ_RETURN(file, &tmp, sizeof(tmp));
            _pressure.append(tmp);
        }
    }

#ifdef ALL_VERSION
    // load point
    if(version < 2){
        point_s_ver_0_ver_1 point;
        point_s tmp;
        SOURCE_READ_RETURN(file, &point, sizeof(point));

        tmp._x = point._x;
        tmp._y = point._y;

        this->_point.append(tmp);
        this->_pressure.append(point.pressure);
    }else
#endif
    {
        // version 2
        for(i = 0; i < len_point; i++){
            SOURCE_READ_RETURN(file, &point_append, sizeof(point_append));
            _point.append(point_append);
        }
    }

#ifdef ALL_VERSION
    if(unlikely(page_point)){
        return PAGE_POINT;
    }
#endif

    this->modify();
    return OK;
}

void stroke::setMetadata(cint posizione_audio, const colore_s &color)
{
    _metadata.posizione_audio = posizione_audio;
    memcpy(&_metadata.color, &color, sizeof(color));
}

size_t stroke::createControll() const
{
    size_t controll = 0;
    int i, len;

    if(unlikely(!this->is_normal())){
        return 0;
    }

    len = length();

    for(i = 0; i < NCOLOR; i++){
        controll += _metadata.color.colore[i];
    }

    for(i = 0; i < len; i++){
        const point_s &point = _point.at(i);
        controll += point._x;
        controll += point._y;
        controll += getPressure(i);
    }

    return controll;
}

size_t stroke::getSizeInMemory() const
{
    return sizeof(point_s) * length();
}

void stroke::decreasePrecision()
{
    int i, len;
    len = length();

    for(i = 1; i < len - 1; i++){
        if(i % 2 == 0){
            this->removeAt(i);
        }
    }

    setFlag(UPDATE_PANTER_PATH, true);
}

void stroke::movePoint(const QPointF &translation)
{
    uint i;
    const uint len = this->length();

    for(i = 0; i < len; i++){
        point_s &point = at_mod(i);
        point._x += translation.x();
        point._y += translation.y();
    }

    if(likely(!needToCreatePanterPath())){
        _path.translate(translation * PROP_RESOLUTION);
    }
    else{
        setFlag(UPDATE_PANTER_PATH, true);
    }
}

void stroke::createQPainterPath(int page) const
{
    auto &__path = (QPainterPath &)     _path;

    constexpr double delta = PROP_RESOLUTION;
    int i = 0, len;

    point_s point1, point2, point3;
    QPointF draw1, draw2, draw3;

    __path = QPainterPath();
    len = this->length();

    Q_ASSERT(len > 2);

    point1 = page::at_translation(at(i),    page);
    point2 = page::at_translation(at(i+1),  page);

    draw1 = point1.toQPointF(delta);
    draw2 = point1.toQPointF(delta);

    __path.moveTo(draw1);
    i = 2;

    for(; i < len - 3; i++){
        point3 =    page::at_translation(at(i), page);
        draw3  =    point3.toQPointF(delta);

        __path.cubicTo(draw1, draw2, draw3);

        point1 = point2; draw1 = draw2;
        point2 = point3; draw2 = draw3;
    }

    setFlag(UPDATE_PANTER_PATH, false);
}

void stroke::reset()
{
    _flag = UPDATE_BIGGER_DATA | UPDATE_PANTER_PATH | UPDATE_PRESSURE;

    /*
     * as we do not know the type of stroke
     * it will become, they week it as normal
     */

    if(likely(is_normal())){
        W_ASSERT(!_complex);
    }else{
        W_ASSERT(_complex);
        free(_complex);
        _complex = NULL;
    }

    _prop = COMPLEX_NORMAL;
    _pressure.clear();
    _point.clear();
    _path = QPainterPath();

    W_ASSERT(this->is_normal());
    W_ASSERT(!_complex);
}

bool stroke::cmp(const stroke &stroke1, const stroke &stroke2)
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

const point_s &stroke::last() const
{
    return _point.last();
}

void stroke::draw(
        QPainter    &painter,
        cbool       is_rubber,
        cint        page,
        QPen        &pen,
        cdouble     prop) const
{
    stroke_drawer::draw_stroke(painter, *this, page, pen, is_rubber, prop);
}
