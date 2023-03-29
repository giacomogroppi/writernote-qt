#include "PenMethod.h"
#include <utility>

PenMethod::PenMethod(std::function<pressure_t(double)> getSize,
                     std::function<int()> getTime,
                     std::function<void()> objectMove,
                     QColor &color, QPen &pen) :
        InsertTools(std::move(getTime),
                    std::move(getSize),
                    std::move(objectMove),
                    color,
                    pen)
{
}

bool PenMethod::touchBegin(const QPointF &point, double size, Document &doc)
{
    return InsertTools::touchBegin(point, size, doc);
}

bool PenMethod::touchUpdate(const QPointF &point, double size, Document &doc)
{
    return InsertTools::touchUpdate(point, size, doc);
}

int PenMethod::touchEnd(const QPointF &point, Document &doc)
{
    return InsertTools::touchEnd(point, doc);
}

uchar PenMethod::getAlfa() const
{
    return 255;
}
