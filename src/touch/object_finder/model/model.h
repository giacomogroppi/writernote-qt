#ifndef MODEL_H
#define MODEL_H

#include <QPointF>
#include <QRectF>
#include "utils/common_def.h"
#include "utils/common_script.h"
#include "zip.h"

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
bool stroke_complex_cmp(const stroke *str1, const stroke *str2);
QRect stroke_complex_bigger_data(const stroke *stroke);

int stroke_complex_save(const stroke *stroke, zip_source_t *file);
int stroke_complex_load(stroke *stroke, int type, zip_file_t *file);

void stroke_complex_append(stroke *stroke, const QPointF &point);
bool stroke_complex_is_inside(const stroke *stroke, const class WLine &line, cdouble precision);

#endif // MODEL_H
