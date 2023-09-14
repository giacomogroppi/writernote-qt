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

    auto touchBegin(const PointF& point, double size, Document &doc) -> UpdateEvent final;
    auto touchUpdate(const PointF& point, double size, Document &doc) -> UpdateEvent final;
    auto touchEnd(const PointF& point, Document &doc) -> UpdateEvent final;

    [[nodiscard]] unsigned char getAlfa() const final;
};

