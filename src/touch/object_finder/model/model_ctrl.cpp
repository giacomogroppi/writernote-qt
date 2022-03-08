#include "touch/object_finder/model/model.h"
#include "touch/datastruct/stroke_complex_data.h"
#include "touch/datastruct/stroke.h"
#include "touch/datastruct/datastruct.h"

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

void *stroke_complex_allocate(int type, void *data)
{
    void *new_data;
    const size_t size = get_size_by_type(type);

    if(type == stroke::COMPLEX_NORMAL){
        new_data = NULL;
        return new_data;
    }

    new_data = malloc(size);
    memcpy(new_data, data, size);

    return new_data;
}

void stroke_complex_adjust(stroke *stroke, cdouble zoom)
{
    W_ASSERT(!stroke->is_normal());

    if(stroke->is_circle()){
        stroke_complex_circle *data = (stroke_complex_circle *)stroke->_complex;
        data->_r /= zoom;
        data->_x /= zoom;
        data->_y /= zoom;
    }else if(stroke->is_line()){
        stroke_complex_line *data = (stroke_complex_line *) stroke->_complex;
        data->bottomRight /= zoom;
        data->topLeft /= zoom;
    }else if(stroke->is_rect()){
        stroke_complex_rect *data = (stroke_complex_rect *) stroke->_complex;
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
        return datastruct_rect(data->topLeft, data->bottomRight).toRect();
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
