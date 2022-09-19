#include "core/WReadZip.h"
#include "touch/object_finder/model/model.h"
#include "touch/datastruct/stroke_complex_data.h"
#include "touch/datastruct/stroke.h"
#include "touch/datastruct/datastruct.h"
#include "dataread/xmlstruct.h"
#include "datawrite/savefile.h"
#include "datawrite/source_read_ext.h"
#include "testing/memtest.h"

static size_t get_size_by_type(int type)
{
    switch (type) {
    case stroke::COMPLEX_CIRCLE:
        return sizeof(stroke_complex_circle);
    case stroke::COMPLEX_LINE:
        return sizeof(stroke_complex_line);
    case stroke::COMPLEX_RECT:
        return sizeof(stroke_complex_rect);
    default:
        std::abort();
    }
}

void *stroke_complex_allocate(int type, const void *data)
{
    void *new_data;

    if(type == stroke::COMPLEX_NORMAL){
        new_data = NULL;
        return new_data;
    }

    const size_t size = get_size_by_type(type);

    new_data = WMalloc(size);
    memcpy(new_data, data, size);

    return new_data;
}

void stroke_complex_adjust(stroke *stroke, cdouble zoom)
{
    W_ASSERT(!stroke->is_normal());

    if(stroke->is_circle()){
        stroke_complex_circle *data = (stroke_complex_circle *)stroke->get_complex_data();
        data->_r /= zoom;
        data->_x /= zoom;
        data->_y /= zoom;
    }else if(stroke->is_line()){
        stroke_complex_line *data = (stroke_complex_line *) stroke->get_complex_data();
        data->pt2 /= zoom;
        data->pt1 /= zoom;
        data->press /= zoom;
    }else if(stroke->is_rect()){
        stroke_complex_rect *data = (stroke_complex_rect *) stroke->get_complex_data();
        const QPointF topLeft = data->rect.topLeft() / zoom;
        const QPointF bottomRight = data->rect.bottomRight() / zoom;
        data->rect = QRectF(topLeft, bottomRight);
    }else{
        std::abort();
    }
}

QRect stroke_complex_bigger_data(const stroke *stroke)
{
    if(stroke->is_line()){
        const auto *data = (const stroke_complex_line *)stroke->get_complex_data();
        return datastruct_rect(data->pt1, data->pt2).toRect();
    }else if(stroke->is_rect()){
        const auto *data = (const stroke_complex_rect *)stroke->get_complex_data();
        return data->rect.toRect();
    }else if(stroke->is_circle()){
        const auto *data = (const stroke_complex_circle *)stroke->get_complex_data();
        const auto topLeft = QPoint(data->_x - data->_r, data->_y - data->_r);
        const auto bottomRight = QPoint(data->_x + data->_r, data->_y + data->_r);
        return QRect(topLeft, bottomRight);
    }else{
        std::abort();
    }
}

bool stroke_complex_cmp(const stroke *str1, const stroke *str2)
{
    const size_t size = get_size_by_type(str1->get_type());

    if(str1->get_type() != str2->get_type())
        return false;

    return (memcmp(str1->get_complex_data(), str2->get_complex_data(), size)) == 0;
}

typedef uchar ver_stroke_complex;
constexpr ver_stroke_complex _current_ver = 0;

int stroke_complex_save(const stroke *stroke, WZipWriterSingle &writer)
{
    const auto type = stroke->get_type();
    const auto size = get_size_by_type(type);
    const auto *data = stroke->get_complex_data();

    static_assert(sizeof(_current_ver) == sizeof(unsigned char));

    writer.write_object(_current_ver);
    writer.write(data, size);

    return OK;
}

#ifdef ALL_VERSION
int stroke_complex_load(stroke *stroke, int type, zip_file_t *filezip)
{
    ver_stroke_complex current_ver;
    const auto size = get_size_by_type(type);
    void *data;

    SOURCE_READ_RETURN_SIZE(filezip, &current_ver, sizeof(current_ver));

    if(current_ver == _current_ver){
        data = WMalloc(size);
        SOURCE_READ_RETURN_SIZE(filezip, data, size);
        stroke->set_complex(type, data);
    }else{
        return ERROR_VERSION;
    }

    return OK;
}
#endif // ALL_VERSION

static int stroke_complex_read_object_size(void *object, WReadZip &reader, int id, size_t size)
{
    const auto *r = reader.read(size, id);
    if(unlikely(!r))
        return -1;

    WMemcpy(&object, r, size);
    return 0;
}

template <class T>
static int stroke_complex_read_object(T &object, WReadZip &reader, int id)
{
    return stroke_complex_read_object_size(&object, reader, id, sizeof(T));
}

#define MANAGE_ERR() return ERROR;

int stroke_complex_load(stroke *stroke, int type, class WReadZip &reader, int id)
{
    W_ASSERT(id >= 0);
    W_ASSERT(stroke);
    ver_stroke_complex current_ver;
    const auto size = get_size_by_type(type);
    void *data;

    if(stroke_complex_read_object(current_ver, reader, id))
        MANAGE_ERR();


    if(current_ver == _current_ver){
        data = WMalloc(size);
        if(stroke_complex_read_object_size(data, reader, id, size)){
            WFree(data);
            MANAGE_ERR();
        }

        stroke->set_complex(type, data);
    }else{
        return ERROR_VERSION;
    }

    return OK;
}


extern void stroke_complex_line_append(stroke *stroke, const QPointF& point);
extern void stroke_complex_circle_append(stroke *stroke, const QPointF& point);
extern void stroke_complex_rect_append(stroke *stroke, const QPointF& point);

void stroke_complex_append(stroke *stroke, const QPointF& point)
{
    W_ASSERT(!stroke->is_normal());
    if(stroke->is_circle())
        return stroke_complex_circle_append(stroke, point);
    if(stroke->is_line())
        return stroke_complex_line_append(stroke, point);
    if(stroke->is_rect())
        return stroke_complex_rect_append(stroke, point);

    std::abort();
}

extern bool stroke_complex_is_inside_circle (const stroke *stroke, const WLine &line, cdouble precision);
extern bool stroke_complex_is_inside_line   (const stroke *stroke, const WLine &line, cdouble precision);
extern bool stroke_complex_is_inside_rect   (const stroke *stroke, const WLine &line, cdouble precision);
bool stroke_complex_is_inside(const stroke *stroke, const WLine &line, cdouble precision)
{
    switch (stroke->get_type()) {
        case stroke::COMPLEX_CIRCLE:
            return stroke_complex_is_inside_circle(stroke, line, precision);
        case stroke::COMPLEX_LINE:
            return stroke_complex_is_inside_line(stroke, line, precision);
        case stroke::COMPLEX_RECT:
            return stroke_complex_is_inside_rect(stroke, line, precision);
        default:
            std::abort();
    }
}

extern void stroke_complex_translate_circle (stroke *stroke, const QPointF &offset);
extern void stroke_complex_translate_line   (stroke *stroke, const QPointF &offset);
extern void stroke_complex_translate_rect   (stroke *stroke, const QPointF &offset);
void stroke_complex_translate(stroke *stroke, const QPointF &offset)
{
    switch (stroke->get_type()) {
        case stroke::COMPLEX_CIRCLE:
            return stroke_complex_translate_circle(stroke, offset);
        case stroke::COMPLEX_LINE:
            return stroke_complex_translate_line(stroke, offset);
        case stroke::COMPLEX_RECT:
            return stroke_complex_translate_rect(stroke, offset);
        default:
            std::abort();
    }
}

extern void stroke_complex_make_normal_circle (const stroke *from, stroke *to);
extern void stroke_complex_make_normal_line   (const stroke *from, stroke *to);
extern void stroke_complex_make_normal_rect   (const stroke *from, stroke *to);
void stroke_complex_make_normal(const stroke *from, stroke *to)
{
    switch (from->get_type()) {
        case stroke::COMPLEX_CIRCLE:
            return stroke_complex_make_normal_circle(from, to);
        case stroke::COMPLEX_LINE:
            return stroke_complex_make_normal_line(from, to);
        case stroke::COMPLEX_RECT:
            return stroke_complex_make_normal_rect(from, to);
        default:
            std::abort();
    }
}

extern bool stroke_complex_is_inside_circle (const stroke *_stroke, const QRectF &area, cdouble precision);
extern bool stroke_complex_is_inside_line   (const stroke *_stroke, const QRectF &area, cdouble precision);
extern bool stroke_complex_is_inside_rect   (const stroke *_stroke, const QRectF &area, cdouble precision);
bool stroke_complex_is_inside(const stroke *_stroke, const QRectF &area, cdouble precision)
{
    switch ( _stroke->get_type() ) {
        case stroke::COMPLEX_CIRCLE:
            return stroke_complex_is_inside_circle(_stroke, area, precision);
        case stroke::COMPLEX_LINE:
            return stroke_complex_is_inside_line(_stroke, area, precision);
        case stroke::COMPLEX_RECT:
            return stroke_complex_is_inside_rect(_stroke, area, precision);
        default:
            std::abort();
    }
}

