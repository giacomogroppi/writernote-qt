#pragma once

#include "touch/tools/InsertTools.h"
#include "currenttitle/document.h"

class PenMethod: public InsertTools
{
private:
public:
    PenMethod(std::function<pressure_t (double size)> getSize,
              std::function<int()> getTime,
              std::function<void()> objectMove,
              QColor &color,
              QPen &pen);

    bool touchBegin(const QPointF& point, double size, Document &doc) final;
    bool touchUpdate(const QPointF& point, double size, Document &doc) final;
    int touchEnd(const QPointF& point, Document &doc) final;

    [[nodiscard]] uchar getAlfa() const final;
};

