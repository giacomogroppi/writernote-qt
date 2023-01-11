#pragma once

#include "touch/tools/Tools.h"
#include "currenttitle/document.h"

class PenMethod: public Tools
{
private:
    /**
     * the function must return the stroke size
     * */
    std::function<pressure_t (double size)>    _getSize;

    /**
     * _getTime must return the time in seconds from the beginning of the recording
     * */
    std::function<qint64()> _getTime;
    std::function<QColor()> _getColor;
    QPen &_pen;
public:
    PenMethod(std::function<pressure_t (double size)> getSize,
              std::function<qint64()> getTime,
              std::function<QColor()> getColor,
              QPen &pen);

    bool touchBegin(const QPointF& point, double size, Document &doc) final;
    bool touchUpdate(const QPointF& point, double size, Document &doc) final;
    int touchEnd(const QPointF& point, Document &doc) final;
};

