#pragma once

#include "touch/tools/InsertTools.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"

class HighligterMethod: public InsertTools {
private:
public:
    HighligterMethod(std::function<int()> getTime,
                     std::function<pressure_t(double)> getSize,
                     std::function<void(const QPointF &)> objectMove,
                     QPen &pen,
                     QColor &color);

    bool touchBegin(const QPointF& point, double size, Document &doc) final;
    bool touchUpdate(const QPointF& point, double size, Document &doc) final;
    int touchEnd(const QPointF& point, Document &doc) final;
};


