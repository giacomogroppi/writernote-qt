#include "stroke.h"
#include "page.h"
#include "utils/common_error_definition.h"
#include "dataread/xmlstruct.h"
#include "datawrite/source_read_ext.h"
#include "currenttitle/document.h"
#include <QDebug>

stroke::stroke()
{
    reset();
}

stroke::stroke(const stroke &data)
{
    reset();
    *this = data;
}

void stroke::__setPressureFirstPoint(cdouble pres)
{
    if(this->length()){
        at_mod(0).pressure = pres;
    }
}

void stroke::__setPressureForAllPoint(const double pressure)
{
    uint i, lenPoint;
    lenPoint = length();

    for(i = 0; i < lenPoint; i++){
        this->at_mod(i).pressure = pressure;
    }
}

int stroke::save(zip_source_t *file) const
{
    int i;
    cint len_point = this->length();

    SOURCE_WRITE_RETURN(file, &len_point, sizeof(len_point));
    SOURCE_WRITE_RETURN(file, &_metadata, sizeof(_metadata));

    for(i = 0; i < len_point; i ++){
        SOURCE_WRITE_RETURN(file, &at(i), sizeof(point_s));
    }

    return OK;
}

int stroke::load(zip_file_t *file, int version)
{
    int i, len_point;
    point_s point_append;

#ifdef ALL_VERSION
    bool page_point = false;

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

    for(i = 0; i < len_point; i++){
        SOURCE_READ_RETURN(file, &point_append, sizeof(point_append));
        this->append(point_append);
    }

#ifdef ALL_VERSION
    if(unlikely(page_point)){
        return PAGE_POINT;
    }
#endif

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
    int i;
    for(i = 0; i < NCOLOR; i++){
        controll += _metadata.color.colore[i];
    }

    for(i = 0; i < length(); i++){
        const point_s &point = _point.at(i);
        controll += point.m_x;
        controll += point.m_y;
        controll += point.pressure;
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

    _needToCreatePanterPath = true;
}

void stroke::movePoint(const QPointF &translation)
{
    uint i;
    const uint len = this->length();

    for(i = 0; i < len; i++){
        point_s &point = at_mod(i);
        point.m_x += translation.x();
        point.m_y += translation.y();
    }

    if(likely(!_needToCreatePanterPath)){
        _path.translate(translation * PROP_RESOLUTION);
    }
    else{
        _needToCreatePanterPath = true;
    }
}

void stroke::createQPainterPath(int page) const
{
    auto &__path = (QPainterPath &)     _path;
    bool &__needToCreatePanterPath = (bool &)   _needToCreatePanterPath;

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

    //__path = __path.simplified();

    __needToCreatePanterPath = false;
}

void stroke::reset()
{
    _needToCreateBiggerData = true;
    _needToCreatePanterPath = true;
    _needToUpdatePressure = true;

    _constantPressureVal = false;

    _point.clear();
    _path = QPainterPath();
}

bool stroke::cmp(const stroke &stroke1, const stroke &stroke2)
{
    int i;
    const int len = stroke1.length();
    if(len != stroke2.length())
        return false;
    for (i = 0; i < len; i ++){
        const point_s &point1 = stroke1.at(i);
        const point_s &point2 = stroke2.at(i);
        if(memcmp(&point1, &point2, sizeof(point1)) != 0){
            return false;
        }
    }

    return true;
}

const point_s &stroke::last() const
{
    return _point.last();
}
