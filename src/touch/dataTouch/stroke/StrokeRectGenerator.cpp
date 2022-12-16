#include "StrokeRectGenerator.h"
#include "touch/dataTouch/stroke/StrokeRect.h"
#include "utils/WCommonScript.h"

std::shared_ptr<Stroke> StrokeRectGenerator::make(const StrokePre *from)
{
    W_ASSERT(0);
    return nullptr;
}

double StrokeRectGenerator::model_near(const StrokePre &stroke)
{
    return StrokeComplexCommon::error;
}
