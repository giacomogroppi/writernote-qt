#ifndef MODEL_H
#define MODEL_H

#include <QPointF>
#include "utils/common_def.h"
#include "utils/common_script.h"

class stroke;

enum{
    COMPLEX_LINE,
    COMPLEX_RECT,
    COMPLEX_CIRCLE
};

double model_line(const stroke *stroke);
double model_rect(const stroke *stroke);
double model_circle(const stroke *stroke);

void model_line_create(stroke *stroke);
void model_rect_create(stroke *stroke);
void model_circle_create(stroke *stroke);

#endif // MODEL_H
