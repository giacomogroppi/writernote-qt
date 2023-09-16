#pragma once

#include "Stroke.h"
#include "StrokeComplexCommon.h"
#include "StrokePre.h"

class StrokeRectGenerator
{
private:
    static constexpr bool StrokeRectGeneratorDebug = true;

    static bool   is_near_rect_x(const RectF &area, const PointF &point);
    static bool   is_near_rect_y(const RectF &area, const PointF &point);
    static double is_near_rect(const RectF &area, const PointF &point);

public:
    static auto make(const WListFast<PointF>& points,
                                        const WListFast<pressure_t>& pressures,
                                        const RectF& area) -> UniquePtr<Stroke>;
    static auto model_near(const WListFast<PointF> &points, const WListFast<pressure_t> &pressures,
                             const RectF &area) -> double;

};
