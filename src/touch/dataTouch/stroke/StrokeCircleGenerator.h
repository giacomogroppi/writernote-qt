#pragma once

#include "Stroke.h"

class StrokeCircleGenerator
{
private:
    static constexpr bool StrokeCircleGeneratorDebug = false;

public:
    static class Stroke *make(const StrokePre *from);
    static double model_near(const class StrokePre &stroke);
};

