#include "StrokeCircleGenerator.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "StrokeNormal.h"
#include "touch/dataTouch/stroke/StrokeCircle.h"
#include "StrokePre.h"

static struct stroke_complex_circle{
    double _x, _y, _r;
} tmp;

Stroke *StrokeCircleGenerator::make(const StrokePre *from)
{
    auto *res = new StrokeCircle();

    W_ASSERT(from);

    res->_data = {
        .x = tmp._x,
        .y = tmp._y,
        .r = tmp._r,
        .press = from->getPressure()
    };

    return res;
}
