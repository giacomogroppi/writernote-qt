#pragma once

#include "touch/tools/Tools.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"

class HighligterMethod: public Tools {
private:
    std::function<pressure_t(double)> _getSize;
    QPen &_pen;
    QColor &_color;
public:
    HighligterMethod(std::function<pressure_t(double)> getSize, QPen &pen, QColor &color);

    bool touchBegin(const QPointF& point, double size, Document &doc) final;
    bool touchUpdate(const QPointF& point, double size, Document &doc) final;
    int touchEnd(const QPointF& point, Document &doc) final;
};


