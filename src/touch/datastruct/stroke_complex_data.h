#ifndef STROKE_COMPLEX_DATA_H
#define STROKE_COMPLEX_DATA_H

#include "utils/common_script.h"
#include <QPainter>
#include <QRect>


struct stroke_complex_circle{
    double _x, _y, _r;
    void draw(QPainter &painter);
    void is_inside(const QPointF& point, cint precision);
};

struct stroke_complex_rect{
    QRect rect;
    void draw(QPainter &painter);
    void is_inside(const QPointF& point, cint precision);
};

#endif // STROKE_COMPLEX_DATA_H
