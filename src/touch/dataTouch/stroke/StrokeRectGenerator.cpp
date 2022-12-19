#include "StrokeRectGenerator.h"
#include "touch/dataTouch/stroke/StrokeRect.h"
#include "utils/WCommonScript.h"
#include "touch/dataTouch/stroke/StrokePre.h"

static QRect area;

bool StrokeRectGenerator::is_near_rect_x(const QRect &area, const point_s &point)
{
    constexpr QPoint delta(50, 0);
    const auto res1 = area.contains(point.toPoint() - delta);
    const auto res2 = area.contains(point.toPoint() + delta);

    if (not ( res1 ^ res2 )) {
        return false;
    }

    return true;
}

bool StrokeRectGenerator::is_near_rect_y(const QRect &area, const point_s &point)
{
    constexpr QPoint delta(0, 50);
    const auto res1 = area.contains(point.toPoint() - delta);
    const auto res2 = area.contains(point.toPoint() + delta);

    if (not ( res1 ^ res2 )) {
        return false;
    }

    return true;
}

double StrokeRectGenerator::is_near_rect(const QRect &area, const point_s &point)
{
    const auto res1 = is_near_rect_x(area, point);
    const auto res2 = is_near_rect_y(area, point);

    if( res1 or res2)
        return 5.;

    return StrokeComplexCommon::error;
}

std::shared_ptr<Stroke> StrokeRectGenerator::make(const StrokePre *from)
{
    std::shared_ptr<StrokeRect> res(new StrokeRect());

    res->_data = {
        .rect = area,
        .press = from->getPressure()
    };

    return res;
}

double StrokeRectGenerator::model_near(const StrokePre &stroke)
{
    using namespace WCommonScript;

    double precision = 0.;
    int err = 0;

    if(!is_near(stroke._point.first(), stroke._point.last(), 30))
        return StrokeComplexCommon::error;

    area = stroke.getBiggerPointInStroke();

    if (std::abs(area.topLeft().y() - area.bottomLeft().y()) < 40)
        return StrokeComplexCommon::error;

    for (const auto &p : stroke._point) {
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

    return precision / stroke._point.length() * 3.;
}
