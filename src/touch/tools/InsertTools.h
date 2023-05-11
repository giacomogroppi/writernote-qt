#pragma once

#include "Tools.h"

class InsertTools: public Tools {
protected:
    std::function<int()> _getTime;
    std::function<pressure_t(double size)> _getSize;
    std::function<void(const QPointF &)> _objectMove;
    QColor &_color;
    QPen &_pen;

    InsertTools(std::function<int()> getTime,
                std::function<pressure_t (double press)> getSize,
                std::function<void(const QPointF&)> objectMove,
                QColor &color,
                QPen &pen);
    virtual ~InsertTools() = default;

    virtual uchar getAlfa() const = 0;

public:
    virtual int touchBegin(const QPointF& point, double size, class Document &doc);
    virtual int touchUpdate(const QPointF& point, double size, class Document &doc);
    virtual int touchEnd(const QPointF& point, class Document &doc);
};

