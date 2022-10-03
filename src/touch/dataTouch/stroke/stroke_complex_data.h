#pragma once

#include "utils/common_script.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include <QPainter>
#include <QRect>


struct stroke_complex_circle{
    double _x, _y, _r;
    pressure_t _press;
};

struct stroke_complex_rect{
    QRectF rect;
    pressure_t press;
};

struct stroke_complex_line{
    QPointF pt1, pt2;
    pressure_t press;
};
