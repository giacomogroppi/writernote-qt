#pragma once

#include "touch/tools/InsertTools.h"
#include "currenttitle/Document.h"

class PenMethod: public InsertTools
{
private:
public:
    PenMethod(std::function<pressure_t (double size)> getSize,
              std::function<int()> getTime,
              QColor &color,
              QPen &pen);

    bool touchBegin(const QPointF& point, double size, Document &doc) final;
    bool touchUpdate(const QPointF& point, double size, Document &doc) final;
    int touchEnd(const QPointF& point, Document &doc) final;

    uchar getAlfa() const final;
};

