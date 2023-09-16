#include "StrokeRectGenerator.h"
#include "touch/dataTouch/stroke/StrokeRect.h"
#include "utils/WCommonScript.h"
#include "touch/dataTouch/stroke/StrokePre.h"

static RectF areaRect;

bool StrokeRectGenerator::is_near_rect_x(const RectF &area, const PointF &point)
{
    constexpr PointF delta(50, 0);
    const auto res1 = area.contains(point - delta);
    const auto res2 = area.contains(point + delta);

    if (not ( res1 ^ res2 )) {
        return false;
    }

    return true;
}

bool StrokeRectGenerator::is_near_rect_y(const RectF &area, const PointF &point)
{
    constexpr PointF delta(0, 50);
    const auto res1 = area.contains(point - delta);
    const auto res2 = area.contains(point + delta);

    if (not ( res1 ^ res2 )) {
        return false;
    }

    return true;
}

double StrokeRectGenerator::is_near_rect(const RectF &area, const PointF &point)
{
    const auto res1 = is_near_rect_x(area, point);
    const auto res2 = is_near_rect_y(area, point);

    if( res1 or res2)
        return 5.;

    return StrokeComplexCommon::error;
}

auto StrokeRectGenerator::make(const WListFast<PointF>& points,
                               const WListFast<pressure_t>& pressures,
                               const RectF& area) -> UniquePtr<Stroke>
{
    UniquePtr<StrokeRect> res(new StrokeRect());

    res->_data = {
        .rect = area,
        .press = pressures.first()
    };

    return res;
}

double StrokeRectGenerator::model_near(const WListFast<PointF> &points, const WListFast<pressure_t> &pressures,
                                       const RectF &area)
{
    using namespace WCommonScript;

    double precision = 0.;
    int err = 0;

    if( !is_near(points.first().x(), points.last().x(), 30) or
        !is_near(points.first().y(), points.last().y(), 30))
        return StrokeComplexCommon::error;

    if (std::abs(area.topLeft().y() - area.bottomLeft().y()) < 40)
        return StrokeComplexCommon::error;

    for (const auto &p : std::as_const(points)) {
        const auto res = is_near_rect(area, p);
        if (res == StrokeComplexCommon::error) {

            WDebug(StrokeRectGeneratorDebug, "err: " << err);

            if(err > 30 and false)
                return StrokeComplexCommon::error;
            err ++;
        }
        precision += res;
    }

    WDebug(StrokeRectGeneratorDebug, precision);

    return precision / points.size() * 3.;
}
