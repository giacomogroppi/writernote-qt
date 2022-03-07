#ifndef MODEL_H
#define MODEL_H

#include <QPointF>
#include <QRectF>
#include "utils/common_def.h"
#include "utils/common_script.h"

constexpr double model_error = 5000.;

class stroke;

double model_line(const stroke *stroke);
double model_rect(const stroke *stroke);
double model_circle(const stroke *stroke);

void model_line_create(stroke *stroke);
void model_rect_create(stroke *stroke);
void model_circle_create(stroke *stroke);

static force_inline not_used bool is_near(const QPointF& p1, const QPointF& p2, cint max)
{
    W_ASSERT(max >= 0);
    const QPointF tl = QPointF(p1.x() - max, p1.y() - max);
    const QPointF br = QPointF(p1.x() + max, p1.y() + max);
    return QRectF(tl, br).contains(p2);
}

void *stroke_complex_allocate(int type, void *data);
void stroke_complex_adjust(stroke *stroke, cdouble zoom);
QRect stroke_complex_bigger_data(const stroke *stroke);

#endif // MODEL_H
