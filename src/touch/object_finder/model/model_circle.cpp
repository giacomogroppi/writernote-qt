#include "touch/dataTouch/stroke/StrokePre.h"
#include "touch/object_finder/model/model.h"
#include "touch/dataTouch/stroke/Stroke.h"
#include "utils/WCommonScript.h"
#include "touch/dataTouch/stroke/stroke_complex_data.h"
#include "testing/memtest.h"
#include "qmath.h"
#include "core/WLine.h"

static struct {
    pressure_t _press;
    double _x, _y, _r;
} circle_data;
constexpr not_used bool debug = false;

static void model_circle_precision(const QPointF &point, double &precision)
{
    double res;
    const auto x = circle_data._x;
    const auto y = circle_data._y;

    /**
     * X^2 + Y^2 - R = res
     */

    res =   WCommonScript::Power(point.x() - x, 2) +
            WCommonScript::Power(point.y() - y, 2)
          - circle_data._r;

    if(qAbs(res) > precision){
        precision = res;
    }
}

double model_circle(const StrokePre *stroke)
{
    const auto area = stroke->getBiggerPointInStroke();
    constexpr auto coef = 500.;
    constexpr auto _end = 10.;
    double precision = 0.;
    double &x = circle_data._x;
    double &y = circle_data._y;
    double &r = circle_data._r;

    {
        const auto rect = stroke->getFirstAndLast();
        if(!is_near(rect.topLeft(), rect.bottomRight(), _end)){
            WDebug(debug, "first point and last are not near" << rect.topLeft() << rect.bottomRight());
            return model_error;
        }
    }

    r = qAbs(area.topLeft().y() - area.bottomRight().y()) / 2.;
    x = (area.topLeft().x() + area.bottomRight().x()) / 2.;
    y = (area.topLeft().y() + area.bottomRight().y()) / 2.;

    W_ASSERT(r >= 0.);
    W_ASSERT(x >= 0.);
    W_ASSERT(y >= 0.);

    for (auto b = stroke->constBegin(); b != stroke->constEnd(); b ++) {
        model_circle_precision(*b, precision);
    }

    precision /= coef;

    WDebug(debug, qstr("Cricle precision: %1").arg(precision));

    return precision;
}

