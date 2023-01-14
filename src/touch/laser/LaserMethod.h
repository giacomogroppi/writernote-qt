#pragma once

#include "touch/tools/InsertTools.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"

class LaserMethod: public InsertTools
{
private:
public:
    LaserMethod(std::function<pressure_t(double)> getPress, QPen &pen, QColor &color);
    ~LaserMethod() = default;
    bool touchBegin(const QPointF& point, double size, class Document &doc) final;
    bool touchUpdate(const QPointF& point, double size, class Document &doc) final;
    int touchEnd(const QPointF& point, class Document &doc) final;
private:
    uchar getAlfa() const final;
};

