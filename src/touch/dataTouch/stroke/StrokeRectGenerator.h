#pragma once

#include "Stroke.h"
#include "StrokeComplexCommon.h"

class StrokeRectGenerator
{
private:
    static constexpr bool StrokeRectGeneratorDebug = true;

    static bool   is_near_rect_x(const QRect &area, const point_s &point);
    static bool   is_near_rect_y(const QRect &area, const point_s &point);
    static double is_near_rect(const QRect &area, const point_s &point);

public:
    static std::shared_ptr<Stroke> make(const class StrokePre *from);
    static double model_near(const class StrokePre &stroke);

};
