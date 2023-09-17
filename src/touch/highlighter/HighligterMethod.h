#pragma once

#include "touch/tools/InsertTools.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"

class HighligterMethod: public InsertTools {
private:
public:
    HighligterMethod(Fn<int()> getTime,
                     Fn<pressure_t(double)> getSize,
                     WPen &pen,
                     WColor &color);

    auto touchBegin(const PointF& point, double size, Document &doc) -> UpdateEvent final;
    auto touchUpdate(const PointF& point, double size, Document &doc) -> UpdateEvent final;
    auto touchEnd(const PointF& point, Document &doc) -> UpdateEvent final;
};


