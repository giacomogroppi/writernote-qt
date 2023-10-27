#pragma once

#include "Stroke.h"
#include "core/WVector.h"
#include "core/pointer/UniquePtr.h"

class StrokeLineGenerator
{
private:
    static constexpr bool StrokeLineGeneratorDebug = false;
    static void model_line_adjust_m(double &m);
    static void is_near_line(double m, double &max, double q, const PointF *point);

    static void makeVertical(const WListFast<PointF>& points,
                             const WVector<pressure_t>& pressures,
                             const RectF& area, class StrokeLine &res);
    static void makeGeneric(const WListFast<PointF>& points,
                            const WVector<pressure_t>& pressures,
                            const RectF& area, class StrokeLine &res);

public:
    static auto make(const WListFast<PointF>& points,
                     const WVector<pressure_t>& pressures,
                     const RectF& area) -> UniquePtr<Stroke>;

    static auto model_near(const WListFast<PointF> &points, const WVector<pressure_t> &pressures,
                           const RectF &area) noexcept -> double;
};

