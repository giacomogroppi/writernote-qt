#pragma once

#include "Stroke.h"
#include "StrokeComplexCommon.h"

class StrokeRectGenerator
{
private:
    static constexpr bool StrokeRectGeneratorDebug = false;

    static double is_near_rect(const QRect &area, const point_s &point);

public:
    static std::shared_ptr<Stroke> make(const class StrokePre *from);
    static double model_near(const class StrokePre &stroke);

};
