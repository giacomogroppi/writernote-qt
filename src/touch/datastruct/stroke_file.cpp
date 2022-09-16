#include "stroke_file.h"
#include "touch/datastruct/stroke.h"
#include "currenttitle/document.h"
#include "core/WReadZip.h"

#define stroke_file_size_len 4

#ifdef ALL_VERSION
force_inline int stroke_file::load_ver_0(class stroke &_stroke, zip_file_t *file)
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

    SOURCE_READ_RETURN_SIZE(file, &len_point, sizeof(len_point));

    SOURCE_READ_RETURN_SIZE(file, &meta, sizeof(meta));
    memcpy(&_stroke._metadata.color,            &meta.color,          sizeof(_stroke._metadata.color));
    memcpy(&_stroke._metadata.posizione_audio,  &meta.posizione_audio,sizeof(_stroke._metadata.posizione_audio));

    // we don't load sheet from differente version
    if(unlikely(meta.idtratto < 0)){
        page_point = true;
    }

    for(i = 0; i < len_point; i++){

        SOURCE_READ_RETURN_SIZE(file, &point, sizeof(point));

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

force_inline int stroke_file::load_ver_1(class stroke &_stroke, zip_file_t *file)
{
    int i, len_point;

    struct point_s_curr {
        double _x, _y;
        float press;
    };

    point_s_curr point;
    point_s tmp;

    SOURCE_READ_RETURN_SIZE(file, &len_point, sizeof(len_point));
    SOURCE_READ_RETURN_SIZE(file, &_stroke._metadata, sizeof(_stroke._metadata));

    for(i = 0; i < len_point; i++){

        SOURCE_READ_RETURN_SIZE(file, &point, sizeof(point));

        tmp._x = point._x;
        tmp._y = point._y;

        _stroke._point.append(tmp);
        _stroke._pressure.append(point.press);
    }

    _stroke.modify();
    return OK;
}

force_inline int stroke_file::load_ver_2(class stroke &_stroke, zip_file_t *file)
{
    int len_press, len_point;
    pressure_t tmp;
    point_s point_append;

    static_assert(sizeof(len_press) == sizeof(len_point));
    static_assert(sizeof(len_point) == stroke_file_size_len);

    SOURCE_READ_RETURN_SIZE(file, &_stroke._metadata, sizeof(_stroke._metadata));
    SOURCE_READ_RETURN_SIZE(file, &_stroke._prop, sizeof(_stroke._prop));

    if(unlikely(_stroke._prop != stroke::COMPLEX_NORMAL)){
        return stroke_complex_load(&_stroke, _stroke._prop, file);
    }

    SOURCE_READ_RETURN_SIZE(file, &len_point, sizeof(len_point));
    SOURCE_READ_RETURN_SIZE(file, &len_press, sizeof(len_press));

    while(len_press -- > 0){
        SOURCE_READ_RETURN_SIZE(file, &tmp, sizeof(tmp));
        _stroke._pressure.append(tmp);
    }

    while(len_point -- > 0){
        SOURCE_READ_RETURN_SIZE(file, &point_append, sizeof(point_append));
        _stroke._point.append(point_append);
    }

    _stroke.modify();
    return OK;
}

#endif // ALL_VERSION

template <class T>
static int stroke_file_ver_2_load_object(T &object, WReadZip &reader, int id)
{
    const auto *data = reader.read(sizeof(object), id);
    if(!data)
        return 1;
    WMemcpy(&object, data, sizeof(object));
    return 0;
}

#define MANAGE_ERR() return ERROR;

force_inline int stroke_file::load_ver_2(class stroke &_stroke, class WReadZip &reader, int id)
{
    int len_press, len_point;
    pressure_t tmp;
    point_s point_append;

    static_assert(sizeof(len_press) == sizeof(len_point));
    static_assert(sizeof(len_point) == stroke_file_size_len);
    static_assert(sizeof(_stroke._metadata) == 8);
    static_assert(sizeof(_stroke._prop) == 4);

    if(stroke_file_ver_2_load_object(_stroke._metadata, reader, id))
        MANAGE_ERR();

    if(stroke_file_ver_2_load_object(_stroke._prop, reader, id))
        MANAGE_ERR();

    if(unlikely(_stroke._prop != stroke::COMPLEX_NORMAL)){
        return stroke_complex_load(&_stroke, _stroke._prop, reader, id);
    }

    if(stroke_file_ver_2_load_object(len_point, reader, id))
        MANAGE_ERR();
    if(stroke_file_ver_2_load_object(len_press, reader, id))
        MANAGE_ERR();

    while(len_press -- > 0){
        if(stroke_file_ver_2_load_object(tmp, reader, id))
            MANAGE_ERR();

        _stroke._pressure.append(tmp);
    }

    while(len_point -- > 0){
        if(stroke_file_ver_2_load_object(point_append, reader, id))
            MANAGE_ERR();

        _stroke._point.append(point_append);
    }

    _stroke.modify();
    return OK;
}

#ifdef ALL_VERSION
int stroke_file::load(class stroke &_stroke, int version, zip_file_t *file)
{
    if(version > CURRENT_VERSION_STROKE)
        return ERROR_VERSION_NEW;

    _stroke.reset();

    switch (version) {
        case 0:
            return stroke_file::load_ver_0(_stroke, file);
        case 1:
            return stroke_file::load_ver_1(_stroke, file);
        case 2:
            return stroke_file::load_ver_2(_stroke, file);
        default:
            return ERROR_VERSION;
    }

    W_ASSERT(0);
}

#endif // ALL_VERSION

int stroke_file::load(stroke &_stroke, int version, WReadZip &reader, int id)
{
    if(unlikely(version > CURRENT_VERSION_STROKE))
        return ERROR_VERSION_NEW;

    _stroke.reset();

    static_assert(CURRENT_VERSION_STROKE == 2);

    switch (version) {
    case 2:
        return stroke_file::load_ver_2(_stroke, reader, id);
    default:
        return ERROR_VERSION;
    }
    W_ASSERT(0);
}

int stroke_file::save(const class stroke &_stroke, zip_source_t *file)
{
    int i;
    cint len_pressure = _stroke._pressure.length();
    cint len_point    = _stroke._point.length();

    static_assert(sizeof(len_pressure) == sizeof(len_point));
    static_assert(sizeof(len_pressure) == stroke_file_size_len);

    SOURCE_WRITE_RETURN_SIZE(file, &_stroke._metadata, sizeof(_stroke._metadata));
    SOURCE_WRITE_RETURN_SIZE(file, &_stroke._prop, sizeof(_stroke._prop));

    if(_stroke.is_complex()){
        W_ASSERT(_stroke._pressure.isEmpty());
        W_ASSERT(_stroke._point.isEmpty());

        return stroke_complex_save(&_stroke, file);
    }

    SOURCE_WRITE_RETURN_SIZE(file, &len_point, sizeof(len_point));
    SOURCE_WRITE_RETURN_SIZE(file, &len_pressure, sizeof(len_pressure));

    for(i = 0; i < len_pressure; i++){
        SOURCE_WRITE_RETURN_SIZE(file, &_stroke._pressure.at(i), sizeof(pressure_t));
    }

    for(i = 0; i < len_point; i ++){
        SOURCE_WRITE_RETURN_SIZE(file, &_stroke.at(i), sizeof(point_s));
    }

    return OK;
}
