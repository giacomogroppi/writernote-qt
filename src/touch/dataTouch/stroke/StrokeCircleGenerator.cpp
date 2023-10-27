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

    res = WUtils::Power(point.x() - x, 2) +
          WUtils::Power(point.y() - y, 2)
          - tmp._r;

    if(std::abs(res) > precision){
        precision = res;
    }
}

auto StrokeCircleGenerator::make(const WListFast<PointF>& points,
                                 const WVector<pressure_t>& pressures,
                                 const RectF& area) -> UniquePtr<Stroke>
{
    UniquePtr<StrokeCircle> res(new StrokeCircle);

    unused(points);
    unused(area);

    res->_data = {
        .position = { tmp._x, tmp._y },
        .r = tmp._r,
        .press = pressures.first()
    };

    return res;
}

auto StrokeCircleGenerator::model_near(const WListFast<PointF> &points,
                                       const WVector<pressure_t> &pressures,
                                       const RectF &area) -> double
{
    using namespace WUtils;
    constexpr auto coef = 500.;
    constexpr auto _end = 10.;
    double precision = 0.;
    double &x = tmp._x;
    double &y = tmp._y;
    double &r = tmp._r;

    {
        const auto rect = RectF {
                points.first(),
                points.last()
        };

        if (!is_near(rect.topLeft(), rect.bottomRight(), _end)) {
            WDebug(StrokeCircleGeneratorDebug, "first point and last are not near" << rect.topLeft() << rect.bottomRight());
            return StrokeComplexCommon::error;
        }
    }

    r = std::abs(area.topLeft().y() - area.bottomRight().y()) / 2.;
    x = (area.topLeft().x() + area.bottomRight().x()) / 2.;
    y = (area.topLeft().y() + area.bottomRight().y()) / 2.;

    W_ASSERT(r >= 0.);
    W_ASSERT(x >= 0.);
    W_ASSERT(y >= 0.);

    for (const auto &point: std::as_const(points)) {
        model_circle_precision(point, precision);
    }

    precision /= coef;

    WDebug(StrokeCircleGeneratorDebug, qstr("Cricle precision: %1").arg(precision));

    return precision;
}
