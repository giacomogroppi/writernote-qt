#pragma once

#include <QPointF>
#include <QRectF>
#include "core/WZipWriterSingle.h"
#include "utils/common_def.h"
#include "utils/common_script.h"
#include "core/WZipReaderSingle.h"

constexpr double model_error = 5000.;

class Stroke;

double model_line(const Stroke *stroke);
double model_rect(const Stroke *stroke);
double model_circle(const Stroke *stroke);

void model_line_create(Stroke *stroke);
void model_rect_create(Stroke *stroke);
void model_circle_create(Stroke *stroke);

static force_inline not_used bool is_near(const QPointF& p1, const QPointF& p2, cint max)
{
    W_ASSERT(max >= 0);
    const QPointF tl = QPointF(p1.x() - max, p1.y() - max);
    const QPointF br = QPointF(p1.x() + max, p1.y() + max);
    return QRectF(tl, br).contains(p2);
}

void *stroke_complex_allocate(int type, const void *data);
void stroke_complex_adjust(Stroke *stroke, cdouble zoom);
bool stroke_complex_cmp(const Stroke *str1, const Stroke *str2);
QRect stroke_complex_bigger_data(const Stroke *stroke);

size_t stroke_complex_get_size_save(const Stroke *stroke);
int stroke_complex_save(const Stroke *stroke, WZipWriterSingle &file);

int stroke_complex_load(Stroke *stroke, int type, WZipReaderSingle &reader);

void stroke_complex_append(Stroke *stroke, const QPointF &point);
bool stroke_complex_is_inside(const Stroke *stroke, const class WLine &line, cdouble precision);
bool stroke_complex_is_inside(const Stroke *stroke, const QRectF &area, cdouble precision);
void stroke_complex_translate(Stroke *stroke, const QPointF &offset);
void stroke_complex_make_normal(const Stroke *from, Stroke *to);

