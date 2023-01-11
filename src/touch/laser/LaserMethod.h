#pragma once

#include "touch/tools/Tools.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"

class LaserMethod: public Tools
{
private:
    std::function<pressure_t()> _getPress;
public:
    LaserMethod(std::function<pressure_t()> getPress);
    ~LaserMethod() = default;
    bool touchBegin(const QPointF& point, double size, class Document &doc) final;
    bool touchUpdate(const QPointF& point, double size, class Document &doc) final;
    int touchEnd(const QPointF& point, class Document &doc) final;
};

