#pragma once

#include "Stroke.h"
#include "core/pointer/UniquePtr.h"

class StrokeLineGenerator
{
private:
    static constexpr bool StrokeLineGeneratorDebug = false;
    static void model_line_adjust_m(double &m);
    static void is_near_line(double m, double &max, double q, const PointF *point);

    static void makeVertical(const WListFast<PointF>& points,
                             const WListFast<pressure_t>& pressures,
                             const RectF& area, class StrokeLine &res);
    static void makeGeneric(const WListFast<PointF>& points,
                            const WListFast<pressure_t>& pressures,
                            const RectF& area, class StrokeLine &res);

public:
    static UniquePtr<Stroke> make(const WListFast<PointF>& points,
                                        const WListFast<pressure_t>& pressures,
                                        const RectF& area);
    static double model_near(const WListFast<PointF> &points, const WListFast<pressure_t> &pressures,
                             const RectF &area) noexcept;
};

