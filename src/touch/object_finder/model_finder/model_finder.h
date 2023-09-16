#pragma once

#include "core/pointer/UniquePtr.h"
#include "core/WListFast.h"
#include "core/PointF.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"

class StrokePre;

class model
{
public:
    static auto find(const WListFast <PointF> &points,
                                   const WListFast <pressure_t> &pressures,
                                   const RectF &area) -> UniquePtr <Stroke> ;
};

