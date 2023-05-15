#pragma once

#include "Tools.h"

class InsertTools: public Tools {
protected:
    std::function<int()> _getTime;
    std::function<pressure_t(double size)> _getSize;
    std::function<void(const PointF &)> _objectMove;
    colore_s &_color;
    WPen &_pen;

    InsertTools(std::function<int()> getTime,
                std::function<pressure_t (double press)> getSize,
                std::function<void(const PointF&)> objectMove,
                colore_s &color,
                WPen &pen);
    virtual ~InsertTools() = default;

    virtual uchar getAlfa() const = 0;

public:
    virtual int touchBegin(const PointF& point, double size, class Document &doc);
    virtual int touchUpdate(const PointF& point, double size, class Document &doc);
    virtual int touchEnd(const PointF& point, class Document &doc);
};

