#include "StrokeRectGenerator.h"
#include "utils/WCommonScript.h"

Stroke *StrokeRectGenerator::make(const StrokePre *from)
{
    W_ASSERT(0);
    return nullptr;
}

double StrokeRectGenerator::model_near(const StrokePre &stroke)
{
    return StrokeComplexCommon::error;
}
