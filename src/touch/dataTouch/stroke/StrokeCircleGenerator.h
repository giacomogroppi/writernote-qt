#pragma once

#include "Stroke.h"
#include "touch/dataTouch/stroke/StrokeComplexCommon.h"

class StrokeCircleGenerator
{
private:
    static constexpr bool StrokeCircleGeneratorDebug = false;
    static void model_circle_precision(const QPointF &point, double &precision);
public:
    static std::shared_ptr<Stroke> make(const class StrokePre *from);
    static double model_near(const class StrokePre &stroke);
};
