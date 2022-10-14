#include "core/WReadZip.h"
#include "touch/object_finder/model/model.h"
#include "touch/dataTouch/stroke/stroke_complex_data.h"
#include "touch/dataTouch/stroke/Stroke.h"
#include "testing/memtest.h"
#include "utils/common_error_definition.h"

static size_t get_size_by_type(StrokeProp type)
{
    switch (type.getProp()) {
    case Stroke::COMPLEX_CIRCLE:
        return sizeof(stroke_complex_circle);
    case Stroke::COMPLEX_LINE:
        return sizeof(stroke_complex_line);
    case Stroke::COMPLEX_RECT:
        return sizeof(stroke_complex_rect);
    default:
        std::abort();
    }
}

void *stroke_complex_allocate (StrokeProp type, const void *data)
{
    void *new_data;

    if(type.is_normal()){
        new_data = nullptr;
        return new_data;
    }

    const size_t size = get_size_by_type(type);

    new_data = WMalloc(size);
    memcpy(new_data, data, size);

    return new_data;
}

void stroke_complex_adjust(Stroke *stroke, cdouble zoom)
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

QRect stroke_complex_bigger_data(const Stroke *stroke)
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

bool stroke_complex_cmp(const Stroke *str1, const Stroke *str2)
{
    const size_t size = get_size_by_type(*str1);

    if(str1->getProp() != str2->getProp())
        return false;

    return (memcmp(str1->get_complex_data(), str2->get_complex_data(), size)) == 0;
}

typedef uchar ver_stroke_complex;
constexpr ver_stroke_complex _current_ver = 0;

int stroke_complex_save(const Stroke *stroke, WZipWriterSingle &writer)
{
    const auto type = stroke->getProp();
    const auto size = get_size_by_type(*stroke);
    const auto *data = stroke->get_complex_data();

    static_assert_type(type, const int);

    static_assert(sizeof(_current_ver) == sizeof(unsigned char));

    writer.write_object(_current_ver);
    writer.write(data, size);

    return OK;
}

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

static int stroke_complex_read_object_size(void *object, WReadZip &reader, int id, size_t size)
{
    const auto *r = reader.read(size, id);
    if(unlikely(!r))
        return -1;

    WMemcpy(&object, r, size);
    return 0;
}

extern void stroke_complex_line_append(     Stroke *stroke, const QPointF& point);
extern void stroke_complex_circle_append(   Stroke *stroke, const QPointF& point);
extern void stroke_complex_rect_append(     Stroke *stroke, const QPointF& point);

void stroke_complex_append(Stroke *stroke, const QPointF& point)
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

extern bool stroke_complex_is_inside_circle (const Stroke *stroke, const WLine &line, cdouble precision);
extern bool stroke_complex_is_inside_line   (const Stroke *stroke, const WLine &line, cdouble precision);
extern bool stroke_complex_is_inside_rect   (const Stroke *stroke, const WLine &line, cdouble precision);
bool stroke_complex_is_inside(const Stroke *stroke, const WLine &line, cdouble precision)
{
    switch (stroke->get_type()) {
        case Stroke::COMPLEX_CIRCLE:
            return stroke_complex_is_inside_circle(stroke, line, precision);
        case Stroke::COMPLEX_LINE:
            return stroke_complex_is_inside_line(stroke, line, precision);
        case Stroke::COMPLEX_RECT:
            return stroke_complex_is_inside_rect(stroke, line, precision);
        default:
            std::abort();
    }
}

extern void stroke_complex_translate_circle (Stroke *stroke, const QPointF &offset);
extern void stroke_complex_translate_line   (Stroke *stroke, const QPointF &offset);
extern void stroke_complex_translate_rect   (Stroke *stroke, const QPointF &offset);
void stroke_complex_translate(Stroke *stroke, const QPointF &offset)
{
    switch (stroke->get_type()) {
        case Stroke::COMPLEX_CIRCLE:
            return stroke_complex_translate_circle(stroke, offset);
        case Stroke::COMPLEX_LINE:
            return stroke_complex_translate_line(stroke, offset);
        case Stroke::COMPLEX_RECT:
            return stroke_complex_translate_rect(stroke, offset);
        default:
            std::abort();
    }
}

extern void stroke_complex_make_normal_circle (const Stroke *from, Stroke *to);
extern void stroke_complex_make_normal_line   (const Stroke *from, Stroke *to);
extern void stroke_complex_make_normal_rect   (const Stroke *from, Stroke *to);
void stroke_complex_make_normal(const Stroke *from, Stroke *to)
{
    switch (from->get_type()) {
        case Stroke::COMPLEX_CIRCLE:
            return stroke_complex_make_normal_circle(from, to);
        case Stroke::COMPLEX_LINE:
            return stroke_complex_make_normal_line(from, to);
        case Stroke::COMPLEX_RECT:
            return stroke_complex_make_normal_rect(from, to);
        default:
            std::abort();
    }
}

extern bool stroke_complex_is_inside_circle (const Stroke *_stroke, const QRectF &area, cdouble precision);
extern bool stroke_complex_is_inside_line   (const Stroke *_stroke, const QRectF &area, cdouble precision);
extern bool stroke_complex_is_inside_rect   (const Stroke *_stroke, const QRectF &area, cdouble precision);
bool stroke_complex_is_inside(const Stroke *_stroke, const QRectF &area, cdouble precision)
{
    switch ( _stroke->get_type() ) {
        case Stroke::COMPLEX_CIRCLE:
            return stroke_complex_is_inside_circle(_stroke, area, precision);
        case Stroke::COMPLEX_LINE:
            return stroke_complex_is_inside_line(_stroke, area, precision);
        case Stroke::COMPLEX_RECT:
            return stroke_complex_is_inside_rect(_stroke, area, precision);
        default:
            std::abort();
    }
}
