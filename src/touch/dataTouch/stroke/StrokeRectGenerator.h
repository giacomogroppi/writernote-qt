#pragma once

#include "Stroke.h"
#include "StrokeComplexCommon.h"

class StrokeRectGenerator
{
private:
    static constexpr bool StrokeRectGeneratorDebug = false;

public:
    static class Stroke *make(const class StrokePre *from);
    static double model_near(const class StrokePre &stroke);

};
