#pragma once

#include "touch/tools/InsertTools.h"
#include "currenttitle/document.h"

class PenMethod: public InsertTools
{
private:
public:
    PenMethod(Fn<pressure_t (double size)> getSize,
              Fn<AudioPosition()> getTime,
              WColor &color,
              WPen &pen);

    auto touchBegin(const PointF& point, double size, Document &doc) -> UpdateEvent final;
    auto touchUpdate(const PointF& point, double size, Document &doc) -> UpdateEvent final;
    auto touchEnd(const PointF& point, Document &doc) -> UpdateEvent final;

    [[nodiscard]] unsigned char getAlfa() const final;
};

