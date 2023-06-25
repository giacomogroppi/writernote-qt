#pragma once

#include "Stroke.h"
#include "touch/dataTouch/stroke/StrokeComplexCommon.h"
#include "StrokePre.h"

class StrokeCircleGenerator
{
private:
    static constexpr bool StrokeCircleGeneratorDebug = false;
    static void model_circle_precision(const PointF &point, double &precision);
public:
    static std::unique_ptr<Stroke> make(const StrokePre *from);
    static double model_near(const class StrokePre &stroke);
};
