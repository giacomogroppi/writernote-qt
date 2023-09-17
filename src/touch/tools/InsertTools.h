#pragma once

#include "Tools.h"

class InsertTools: public Tools {
protected:
    Fn<int()> _getTime;
    Fn<pressure_t(double size)> _getSize;
    WColor &_color;
    WPen &_pen;

    InsertTools(Fn<int()> getTime,
                Fn<pressure_t (double press)> getSize,
                WColor &color,
                WPen &pen);
    virtual ~InsertTools() = default;

    virtual auto getAlfa() const -> unsigned char = 0;

public:
    virtual auto touchBegin(const PointF& point, double size, class Document &doc) -> UpdateEvent;
    virtual auto touchUpdate(const PointF& point, double size, class Document &doc) -> UpdateEvent;
    virtual auto touchEnd(const PointF& point, class Document &doc) -> UpdateEvent;
};

