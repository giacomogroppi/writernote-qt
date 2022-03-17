#include "model.h"
#include "touch/datastruct/datastruct.h"
#include <QString>

constexpr bool not_used debug = true;

double model_rect(const stroke *stroke)
{
    Q_UNUSED(stroke);
    return model_error;
}

void model_rect_create(stroke *stroke)
{
    Q_UNUSED(stroke);
}

void stroke_complex_rect_append(stroke *stroke, const QPointF& point)
{
    Q_UNUSED(stroke);
    Q_UNUSED(point);
    std::abort();
}

bool stroke_complex_is_inside_rect(const stroke *stroke, const WLine &line, cdouble precision)
{
    Q_UNUSED(stroke);
    Q_UNUSED(line);
    std::abort();
}

void stroke_complex_translate_rect(stroke *stroke, const QPointF &offset)
{
    Q_UNUSED(stroke);
    Q_UNUSED(offset);
    std::abort();
}

void stroke_complex_make_normal_rect   (const stroke *_from, stroke *_to)
{
    Q_UNUSED(_from);
    Q_UNUSED(_to);
    std::abort();
}