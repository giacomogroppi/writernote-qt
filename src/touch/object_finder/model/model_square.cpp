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