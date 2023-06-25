#pragma once

#include "Stroke.h"

class StrokeLineGenerator
{
private:
    static constexpr bool StrokeLineGeneratorDebug = false;
    static void model_line_adjust_m(double &m);
    static void is_near_line(double m, double &max, double q, const PointF *point);

    static void makeVertical(const class StrokePre *from, class StrokeLine &res);
    static void makeGeneric(const class StrokePre *from, class StrokeLine &res);

public:
    static std::unique_ptr<Stroke> make(const StrokePre *from);
    static double model_near(const class StrokePre &stroke);
};

