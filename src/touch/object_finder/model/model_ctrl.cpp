#include "touch/object_finder/model/model.h"
#include "touch/datastruct/stroke_complex_data.h"
#include "touch/datastruct/stroke.h"
#include "touch/datastruct/datastruct.h"

void *stroke_complex_allocate(int type, void *data)
{
    void *new_data;

    if(type == stroke::COMPLEX_NORMAL){
        new_data = NULL;
    }
    else if(type == stroke::COMPLEX_LINE){
        new_data = malloc(sizeof(stroke_complex_line));
        memcpy(new_data, data, sizeof(stroke_complex_line));
    }else if(type == stroke::COMPLEX_CIRCLE){
        new_data = malloc(sizeof(stroke_complex_circle));
        memcpy(new_data, data, sizeof(stroke_complex_circle));
    }else if(type == stroke::COMPLEX_RECT){
        new_data = malloc(sizeof(stroke_complex_rect));
        memcpy(new_data, data, sizeof(stroke_complex_rect));
    }else{
        std::abort();
    }

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
