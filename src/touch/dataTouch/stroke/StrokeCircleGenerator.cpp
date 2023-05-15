#include "StrokeCircleGenerator.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "StrokeNormal.h"
#include "touch/dataTouch/stroke/StrokeCircle.h"
#include "StrokePre.h"

static struct stroke_complex_circle{
    double _x, _y, _r;
} tmp;

void StrokeCircleGenerator::model_circle_precision(const PointF &point, double &precision)
{
    double res;
    const auto x = tmp._x;
    const auto y = tmp._y;

    /**
     * X^2 + Y^2 - R = res
     */

    res =   WCommonScript::Power(point.x() - x, 2) +
            WCommonScript::Power(point.y() - y, 2)
          - tmp._r;

    if(qAbs(res) > precision){
        precision = res;
    }
}

std::shared_ptr<Stroke> StrokeCircleGenerator::make(const StrokePre *from)
{
    std::shared_ptr<StrokeCircle> res(new StrokeCircle);

    W_ASSERT(from);

    res->_data = {
        .x = tmp._x,
        .y = tmp._y,
        .r = tmp._r,
        .press = from->getPressure()
    };

    return res;
}

double StrokeCircleGenerator::model_near(const StrokePre &stroke)
{
    using namespace WCommonScript;
    const auto area = stroke.getBiggerPointInStroke();
    constexpr auto coef = 500.;
    constexpr auto _end = 10.;
    double precision = 0.;
    double &x = tmp._x;
    double &y = tmp._y;
    double &r = tmp._r;

    W_ASSERT(stroke._stroke->isEmpty());

    {
        const auto rect = stroke.getFirstAndLast();
        if (!is_near(rect.topLeft(), rect.bottomRight(), _end)) {
            WDebug(StrokeCircleGeneratorDebug, "first point and last are not near" << rect.topLeft() << rect.bottomRight());
            return StrokeComplexCommon::error;
        }
    }

    r = qAbs(area.topLeft().y() - area.bottomRight().y()) / 2.;
    x = (area.topLeft().x() + area.bottomRight().x()) / 2.;
    y = (area.topLeft().y() + area.bottomRight().y()) / 2.;

    W_ASSERT(r >= 0.);
    W_ASSERT(x >= 0.);
    W_ASSERT(y >= 0.);

    for (auto b = stroke.constBegin(); b != stroke.constEnd(); b ++) {
        model_circle_precision(*b, precision);
    }

    precision /= coef;

    WDebug(StrokeCircleGeneratorDebug, qstr("Cricle precision: %1").arg(precision));

    return precision;
}
