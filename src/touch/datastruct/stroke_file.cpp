#include "stroke_file.h"
#include "touch/datastruct/stroke.h"
#include "currenttitle/document.h"
#include "core/WReadZip.h"
#include "core/WZipWriterSingle.h"
#include "datawrite/source_read_ext.h"

#define stroke_file_size_len 4

#ifdef ALL_VERSION
force_inline int stroke_file::load_ver_0(class stroke &_stroke, WZipReaderSingle &reader)
{
    int i, len_point;
    bool page_point = false;
    struct point_s_curr {
        double _x, _y;
        float press;
    };

    struct old_metadata{
        int page, idtratto, posizione_audio;
        struct colore_s color;
    } meta;

    point_s_curr point;
    point_s tmp;

    if(reader.read_object(len_point) < 0)
        return ERROR;

    if(reader.read_object(meta) < 0)
        return ERROR;
    memcpy(&_stroke._metadata.color,            &meta.color,          sizeof(_stroke._metadata.color));
    memcpy(&_stroke._metadata.posizione_audio,  &meta.posizione_audio,sizeof(_stroke._metadata.posizione_audio));

    // we don't load sheet from differente version
    if(unlikely(meta.idtratto < 0)){
        page_point = true;
    }

    for(i = 0; i < len_point; i++){
        if(reader.read_object(point) < 0)
            return ERROR;

        tmp._x = point._x;
        tmp._y = point._y;

        _stroke._point.append(tmp);
        _stroke._pressure.append(point.press);
    }

    if(unlikely(page_point)){
        return PAGE_POINT;
    }

    _stroke.modify();
    return OK;
}

force_inline int stroke_file::load_ver_1(class stroke &_stroke, WZipReaderSingle &reader)
{
    int i, len_point;

    struct point_s_curr {
        double _x, _y;
        float press;
    };

    point_s_curr point;
    point_s tmp;

    if(reader.read_object(len_point) < 0)
        return ERROR;
    if(reader.read_object(_stroke._metadata) < 0)
        return ERROR;

    for(i = 0; i < len_point; i++){
        if(reader.read_object(point) < 0)
            return ERROR;

        tmp._x = point._x;
        tmp._y = point._y;

        _stroke._point.append(tmp);
        _stroke._pressure.append(point.press);
    }

    _stroke.modify();
    return OK;
}

force_inline int stroke_file::load_ver_2(class stroke &_stroke, WZipReaderSingle &reader)
{
    int len_press, len_point;
    pressure_t tmp;
    point_s point_append;

    static_assert(sizeof(len_press) == sizeof(len_point));
    static_assert(sizeof(len_point) == stroke_file_size_len);

    if(reader.read_object(_stroke._metadata) < 0)
        return ERROR;
    if(reader.read_object(_stroke._prop) < 0)
        return ERROR;

    if(unlikely(_stroke._prop != stroke::COMPLEX_NORMAL)){
        return stroke_complex_load(&_stroke, _stroke._prop, reader);
    }

    if(reader.read_object(len_point) < 0)
        return ERROR;
    if(reader.read_object(len_press) < 0)
        return ERROR;

    while(len_press -- > 0){
        if(reader.read_object(tmp) < 0)
            return ERROR;
        _stroke._pressure.append(tmp);
    }

    while(len_point -- > 0){
        if(reader.read_object(point_append) < 0)
            return ERROR;
        _stroke._point.append(point_append);
    }

    _stroke.modify();
    return OK;
}

#endif // ALL_VERSION

#define MANAGE_ERR() return ERROR;

force_inline int stroke_file::load_ver_2(class stroke &_stroke, class WReadZip &readerMulti, int id)
{
    int len_press, len_point;
    pressure_t tmp;
    point_s point_append;
    WZipReaderSingle &reader = *readerMulti.get_reader(id);

    static_assert(sizeof(len_press) == sizeof(len_point));
    static_assert(sizeof(len_point) == stroke_file_size_len);
    static_assert(sizeof(_stroke._metadata) == 8);
    static_assert(sizeof(_stroke._prop) == 4);

    if(reader.read_object(_stroke._metadata) < 0)
        MANAGE_ERR();

    if(reader.read_object(_stroke._prop) < 0)
        MANAGE_ERR();

    if(unlikely(_stroke._prop != stroke::COMPLEX_NORMAL)){
        return stroke_complex_load(&_stroke, _stroke._prop, reader);
    }

    if(reader.read_object(len_point) < 0)
        MANAGE_ERR();

    if(reader.read_object(len_press) < 0)
        MANAGE_ERR();

    while(len_press -- > 0){
        if(reader.read_object(tmp) < 0)
            MANAGE_ERR();

        _stroke._pressure.append(tmp);
    }

    while(len_point -- > 0){
        if(reader.read_object(point_append) < 0)
            MANAGE_ERR();

        _stroke._point.append(point_append);
    }

    _stroke.modify();
    return OK;
}

int stroke_file::load(class stroke &_stroke, int version, WZipReaderSingle &reader)
{
    if(version > CURRENT_VERSION_STROKE)
        return ERROR_VERSION_NEW;

    _stroke.reset();

    switch (version) {
        case 0:
            return stroke_file::load_ver_0(_stroke, reader);
        case 1:
            return stroke_file::load_ver_1(_stroke, reader);
        case 2:
            return stroke_file::load_ver_2(_stroke, reader);
        default:
            return ERROR_VERSION;
    }

    W_ASSERT(0);
}

size_t stroke_file::get_size_in_file(const stroke &_stroke)
{
    size_t s = 0;
    int i;
    cint len_pressure = _stroke._pressure.length();
    cint len_point    = _stroke._point.length();

    static_assert(sizeof(len_pressure) == sizeof(len_point));
    static_assert(sizeof(len_pressure) == stroke_file_size_len);

    s += sizeof(_stroke._metadata);
    s += sizeof(_stroke._prop);

    if(_stroke.is_complex()){
        W_ASSERT(_stroke._pressure.isEmpty());
        W_ASSERT(_stroke._point.isEmpty());

        s += stroke_complex_get_size_save(&_stroke);
        return s;
    }

    s += sizeof(len_point);
    s += sizeof(len_pressure);

    s += sizeof(pressure_t)     * len_pressure;
    s += sizeof(point_s)        * len_point;

    return s;
}

int stroke_file::save(const class stroke &_stroke, WZipWriterSingle &writer)
{
    int i;
    cint len_pressure = _stroke._pressure.length();
    cint len_point    = _stroke._point.length();

    static_assert(sizeof(len_pressure) == sizeof(len_point));
    static_assert(sizeof(len_pressure) == stroke_file_size_len);

    writer.write_object(_stroke._metadata);
    writer.write_object(_stroke._prop);

    if(_stroke.is_complex()){
        W_ASSERT(_stroke._pressure.isEmpty());
        W_ASSERT(_stroke._point.isEmpty());

        return stroke_complex_save(&_stroke, writer);
    }

    writer.write_object(len_point);
    writer.write_object(len_pressure);

    for(i = 0; i < len_pressure; i++){
        writer.write(&_stroke._pressure.at(i), sizeof(pressure_t));
    }

    for(i = 0; i < len_point; i ++){
        writer.write(&_stroke.at(i), sizeof(point_s));
    }

    return OK;
}
