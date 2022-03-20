#include "stroke_file.h"
#include "touch/datastruct/stroke.h"
#include "currenttitle/document.h"

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

    SOURCE_READ_RETURN(file, &len_point, sizeof(len_point));

    SOURCE_READ_RETURN(file, &meta, sizeof(meta));
    memcpy(&_stroke._metadata.color,            &meta.color,          sizeof(_stroke._metadata.color));
    memcpy(&_stroke._metadata.posizione_audio,  &meta.posizione_audio,sizeof(_stroke._metadata.posizione_audio));

    // we don't load sheet from differente version
    if(unlikely(meta.idtratto < 0)){
        page_point = true;
    }

    for(i = 0; i < len_point; i++){

        SOURCE_READ_RETURN(file, &point, sizeof(point));

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

    SOURCE_READ_RETURN(file, &len_point, sizeof(len_point));
    SOURCE_READ_RETURN(file, &_stroke._metadata, sizeof(_stroke._metadata));

    for(i = 0; i < len_point; i++){

        SOURCE_READ_RETURN(file, &point, sizeof(point));

        tmp._x = point._x;
        tmp._y = point._y;

        _stroke._point.append(tmp);
        _stroke._pressure.append(point.press);
    }

    _stroke.modify();
    return OK;
}
#endif

force_inline int stroke_file::load_ver_2(class stroke &_stroke, zip_file_t *file)
{
    int len_point, i;
    point_s point_append;

    SOURCE_READ_RETURN(file, &len_point, sizeof(len_point));
    SOURCE_READ_RETURN(file, &_stroke._metadata, sizeof(_stroke._metadata));

    {
        int len_press;
        pressure_t tmp;

        SOURCE_READ_RETURN(file, &_stroke._prop, sizeof(_stroke._prop));
        if(unlikely(_stroke._prop != stroke::COMPLEX_NORMAL)){
            return stroke_complex_load(&_stroke, _stroke._prop, file);
        }

        SOURCE_READ_RETURN(file, &len_press, sizeof(len_press));

        for(i = 0; i < len_press; i++){
            SOURCE_READ_RETURN(file, &tmp, sizeof(tmp));
            _stroke._pressure.append(tmp);
        }

    }

    for(i = 0; i < len_point; i++){
        SOURCE_READ_RETURN(file, &point_append, sizeof(point_append));
        _stroke._point.append(point_append);
    }

    _stroke.modify();
    return OK;
}

int stroke_file::load(class stroke &_stroke, int version, zip_file_t *file)
{
    if(version >= CURRENT_VERSION_STROKE)
        return ERROR_VERSION_NEW;

    _stroke.reset();

#ifdef ALL_VERSION
    switch (version) {
        case 0:
            return stroke_file::load_ver_0(_stroke, file);
        case 1:
            return stroke_file::load_ver_1(_stroke, file);
        case 2:
            return stroke_file::load_ver_2(_stroke, file);
        default:
            return ERROR;
    }
#else
    if(version != CURRENT_VERSION_STROKE){
        return ERROR_VERSION;
    }
    return stroke_file::load_ver_2(_stroke, file);
#endif // ALL_VERSION
}

int stroke_file::save(const class stroke &_stroke, zip_source_t *file)
{

}