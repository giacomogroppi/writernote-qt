#pragma once

#include "touch/tools/InsertTools.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"

class HighligterMethod: public InsertTools {
private:
public:
    HighligterMethod(std::function<int()> getTime,
                     std::function<pressure_t(double)> getSize,
                     std::function<void(const PointF &)> objectMove,
                     WPen &pen,
                     colore_s &color);

    int touchBegin(const PointF& point, double size, Document &doc) final;
    int touchUpdate(const PointF& point, double size, Document &doc) final;
    int touchEnd(const PointF& point, Document &doc) final;
};


