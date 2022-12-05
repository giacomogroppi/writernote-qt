#pragma once

#include "Stroke.h"

class StrokeLineGenerator
{
private:
    static constexpr bool StrokeLineGeneratorDebug = false;
    static void model_line_adjust_m(double &m);
    static void is_near_line(cdouble m, double &max, cdouble q, const point_s *point);

    static void makeVertical(class StrokeNormal *from, class StrokeLine *res);
    static void makeGeneric(class StrokeNormal *from, class StrokeLine *res);

public:
    static class Stroke *make(const StrokePre *from);
    static double model_near(const class StrokePre &stroke);
};

