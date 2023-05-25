#pragma once

#include "touch/tools/InsertTools.h"
#include "currenttitle/document.h"

class PenMethod: public InsertTools
{
private:
public:
    PenMethod(std::function<pressure_t (double size)> getSize,
              std::function<int()> getTime,
              std::function<void(const PointF &point)> objectMove,
              WColor &color,
              WPen &pen);

    int touchBegin(const PointF& point, double size, Document &doc) final;
    int touchUpdate(const PointF& point, double size, Document &doc) final;
    int touchEnd(const PointF& point, Document &doc) final;

    [[nodiscard]] unsigned char getAlfa() const final;
};

