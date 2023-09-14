#include "PenMethod.h"
#include <utility>

PenMethod::PenMethod(std::function<pressure_t(double)> getSize,
                     std::function<int()> getTime,
                     std::function<void(const PointF &point)> objectMove,
                     WColor &color, WPen &pen) :
        InsertTools(std::move(getTime),
                    std::move(getSize),
                    std::move(objectMove),
                    color,
                    pen)
{
}

auto PenMethod::touchBegin(const PointF &point, double size, Document &doc) -> UpdateEvent
{
    return InsertTools::touchBegin(point, size, doc);
}

auto PenMethod::touchUpdate(const PointF &point, double size, Document &doc) -> UpdateEvent
{
    return InsertTools::touchUpdate(point, size, doc);
}

auto PenMethod::touchEnd(const PointF &point, Document &doc) -> UpdateEvent
{
    return InsertTools::touchEnd(point, doc);
}

unsigned char PenMethod::getAlfa() const
{
    return 255;
}
