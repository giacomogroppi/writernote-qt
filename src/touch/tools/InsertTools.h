#pragma once

#include "Tools.h"

class InsertTools: public Tools {
protected:
    std::function<int()> _getTime;
    std::function<pressure_t(double size)> _getSize;
    std::function<void(const PointF &)> _objectMove;
    WColor &_color;
    WPen &_pen;

    InsertTools(std::function<int()> getTime,
                std::function<pressure_t (double press)> getSize,
                std::function<void(const PointF&)> objectMove,
                WColor &color,
                WPen &pen);
    virtual ~InsertTools() = default;

    virtual auto getAlfa() const -> unsigned char = 0;

public:
    virtual auto touchBegin(const PointF& point, double size, class Document &doc) -> UpdateEvent;
    virtual auto touchUpdate(const PointF& point, double size, class Document &doc) -> UpdateEvent;
    virtual auto touchEnd(const PointF& point, class Document &doc) -> UpdateEvent;
};

