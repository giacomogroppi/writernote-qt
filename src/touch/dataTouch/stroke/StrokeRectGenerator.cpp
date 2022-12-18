#include "StrokeRectGenerator.h"
#include "touch/dataTouch/stroke/StrokeRect.h"
#include "utils/WCommonScript.h"
#include "touch/dataTouch/stroke/StrokePre.h"

double StrokeRectGenerator::is_near_rect(const QRect &area, const point_s &point)
{
    constexpr QPoint delta(20, 20);
    if (not (area.contains(point.toPoint() - delta) ^
            area.contains(point.toPoint() + delta)) ) {
        return StrokeComplexCommon::error;
    }

    return 5.;
}

std::shared_ptr<Stroke> StrokeRectGenerator::make(const StrokePre *from)
{
    W_ASSERT(0);
    return nullptr;
}

double StrokeRectGenerator::model_near(const StrokePre &stroke)
{
    using namespace WCommonScript;

    double precision = 0.;

    if(!is_near(stroke._point.first(), stroke._point.last(), 10))
        return StrokeComplexCommon::error;

    const auto &area = stroke.getBiggerPointInStroke();

    if (std::abs(area.topLeft().y() - area.bottomLeft().y()) < 40)
        return StrokeComplexCommon::error;

    for (const auto &p : stroke._point) {
        const auto res = is_near_rect(area, p);
        if (res == StrokeComplexCommon::error) {
            return StrokeComplexCommon::error;
        }
        precision += res;
    }

    return precision;
}
