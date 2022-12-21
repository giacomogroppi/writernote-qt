#pragma once

#include "touch/tools/Tools.h"
#include "currenttitle/document.h"

class PenMethod: public Tools
{
private:
    std::function<int()>    _getSize;
    std::function<qint64()> _getTime;
public:
    PenMethod(std::function<int()> getSize, std::function<qint64()> getTime);
    bool touchBegin(const QPointF& point, Document &doc);
    bool touchUpdate(const QPointF& point, Document &doc);
    int touchEnd(const QPointF& point, Document &doc);
};

