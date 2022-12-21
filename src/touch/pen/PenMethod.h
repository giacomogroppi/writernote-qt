#pragma once

#include "touch/tools/Tools.h"

class PenMethod: public Tools
{
private:
    std::function<int()> _func;
public:
    PenMethod(std::function<int()>);
    bool touchBegin(const QPointF& point);
    bool touchUpdate(const QPointF& point);
    int touchEnd(const QPointF& point);
};

