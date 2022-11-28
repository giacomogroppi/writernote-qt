#include "core/WReadZip.h"
#include "touch/object_finder/model/model.h"
#include "touch/dataTouch/stroke/stroke_complex_data.h"
#include "touch/dataTouch/stroke/Stroke.h"
#include "testing/memtest.h"
#include "utils/common_error_definition.h"

typedef uchar ver_stroke_complex;
constexpr ver_stroke_complex _current_ver = 0;

size_t stroke_complex_get_size_save(const Stroke *stroke)
{
    size_t s = 0;
    const auto size = get_size_by_type(*stroke);

    static_assert(sizeof(_current_ver) == sizeof(unsigned char));

    s += sizeof(_current_ver);
    s += size;

    return s;
}

int stroke_complex_load(Stroke *stroke, StrokeProp type, WZipReaderSingle &reader)
{
    ver_stroke_complex current_ver;
    const auto size = get_size_by_type(type);
    void *data;

    if(reader.read_object(current_ver) < 0)
        return ERROR;

    if(current_ver == _current_ver){
        data = WMalloc(size);
        if(reader.read_by_size(data, size) < 0)
            return ERROR;
        stroke->set_complex(type, data);
    }else{
        return ERROR_VERSION;
    }

    return OK;
}
