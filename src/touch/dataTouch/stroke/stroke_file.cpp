#include "stroke_file.h"
#include "touch/dataTouch/stroke/Stroke.h"
#include "currenttitle/document.h"
#include "core/WReadZip.h"
#include "core/WZipWriterSingle.h"
#include "datawrite/source_read_ext.h"

#define stroke_file_size_len 4

#ifdef ALL_VERSION
force_inline int stroke_file::load_ver_0(class Stroke &_stroke, WZipReaderSingle &reader)
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

        tmp = point_s(point._x, point._y);

        _stroke._point.append(tmp);
        _stroke._pressure.append(point.press);
    }

    if(unlikely(page_point)){
        return PAGE_POINT;
    }

    _stroke.modify();
    return OK;
}

force_inline int stroke_file::load_ver_1(class Stroke &_stroke, WZipReaderSingle &reader)
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

        tmp = point_s(point._x, point._y);

        _stroke._point.append(tmp);
        _stroke._pressure.append(point.press);
    }

    _stroke.modify();
    return OK;
}

#endif // ALL_VERSION

#define MANAGE_ERR() return ERROR

force_inline int stroke_file::load_ver_2(class Stroke &_stroke, WZipReaderSingle &reader)
{
    int i;
    int len_press, len_point;
    pressure_t tmp;
    point_s point_append;

    static_assert(sizeof(len_press) == sizeof(len_point));
    static_assert(sizeof(len_point) == stroke_file_size_len);
    static_assert(sizeof(_stroke._metadata) == 8);

    if(reader.read_by_size(&_stroke._metadata, sizeof(_stroke._metadata)) < 0)
        MANAGE_ERR();
    if(reader.read_object(_stroke.PropRef()) < 0)
        MANAGE_ERR();

    if(unlikely(_stroke.is_complex())){
        return stroke_complex_load(&_stroke, dynamic_cast<StrokeProp &>(_stroke), reader);
    }

    if(reader.read_object(len_point) < 0)
        MANAGE_ERR();
    if(reader.read_object(len_press) < 0)
        MANAGE_ERR();

    W_ASSERT(len_press <= len_point);

    for(i = 0; i < len_press; i++){
        if(reader.read_object(tmp) < 0)
            MANAGE_ERR();
        _stroke._pressure.append(tmp);
    }

    for(i = 0; i < len_point; i++){
        if(reader.read_object(point_append) < 0)
            MANAGE_ERR();
        _stroke._point.append(point_append);
    }

    _stroke.modify();
    return OK;
}

int stroke_file::load(class Stroke &_stroke, int version, WZipReaderSingle &reader)
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

size_t stroke_file::get_size_in_file(const Stroke &stroke)
{
    size_t s = 0;
    int i;
    cint len_pressure = stroke._pressure.length();
    cint len_point    = stroke._point.length();

    static_assert(sizeof(len_pressure) == sizeof(len_point));
    static_assert(sizeof(len_pressure) == stroke_file_size_len);

    static_assert_type(len_pressure, const int);
    static_assert_type(len_point, const int);
    static_assert_type(stroke.getProp(), int);


    s += sizeof(stroke._metadata);
    s += sizeof(stroke.getProp());

    if (stroke.is_complex()) {
        W_ASSERT(stroke._pressure.isEmpty());
        W_ASSERT(stroke._point.isEmpty());

        s += stroke_complex_get_size_save(&stroke);
        return s;
    }

    s += sizeof(len_point);
    s += sizeof(len_pressure);

    s += sizeof(pressure_t)     * len_pressure;
    s += sizeof(point_s)        * len_point;

    return s;
}

int stroke_file::save(const class Stroke &_stroke, WZipWriterSingle &writer)
{
    int i;
    cint len_pressure = _stroke._pressure.length();
    cint len_point    = _stroke._point.length();

    static_assert(sizeof(len_pressure) == sizeof(len_point));
    static_assert(sizeof(len_pressure) == stroke_file_size_len);
    static_assert(sizeof(_stroke._metadata) == 8);
    static_assert_type(_stroke.getProp(), int);

    writer.write(&_stroke._metadata, sizeof(_stroke._metadata));
    writer.write_object(_stroke.getProp());

    if(_stroke.is_complex()){
        W_ASSERT(_stroke._pressure.isEmpty());
        W_ASSERT(_stroke._point.isEmpty());

        return stroke_complex_save(&_stroke, writer);
    }

    writer.write_object(len_point);
    writer.write_object(len_pressure);

    W_ASSERT(len_point >= len_pressure);

    for(i = 0; i < len_pressure; i++){
        writer.write(&_stroke._pressure.at(i), sizeof(pressure_t));
    }

    for(i = 0; i < len_point; i ++){
        writer.write(&_stroke.at(i), sizeof(point_s));
    }

    return OK;
}
