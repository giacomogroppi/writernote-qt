#pragma once

#include "Stroke.h"

class StrokeLineGenerator
{
private:
    static constexpr bool StrokeLineGeneratorDebug = false;
    static void model_line_adjust_m(double &m);
    static void is_near_line(cdouble m, double &max, cdouble q, const Point *point);

    static void makeVertical(const class StrokePre *from, class StrokeLine &res);
    static void makeGeneric(const class StrokePre *from, class StrokeLine &res);

public:
    static std::shared_ptr<Stroke> make(const StrokePre *from);
    static double model_near(const class StrokePre &stroke);
};

