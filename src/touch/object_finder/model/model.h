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

bool model_line(const stroke *stroke);
bool model_rect(const stroke *stroke);
bool model_circle(const stroke *stroke);

#endif // MODEL_H
