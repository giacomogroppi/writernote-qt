#pragma once

#include "Stroke.h"
#include "touch/dataTouch/stroke/StrokeComplexCommon.h"
#include "StrokePre.h"

class StrokeCircleGenerator
{
private:
    static constexpr bool debug = false;
    static void model_circle_precision(const PointF &point, double &precision);
public:
    static auto make(const WListFast<PointF>& points,
                     const WVector<pressure_t>& pressures,
                     const RectF& area) -> UniquePtr<Stroke>;

    // TODO: make this method return a StrokeGeneratorData
    static auto model_near(const WListFast<PointF> &points,
                           const WVector<pressure_t> &pressures,
                           const RectF &area) -> double;
};
