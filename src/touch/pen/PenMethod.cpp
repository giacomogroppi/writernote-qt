#include "PenMethod.h"
#include <utility>

PenMethod::PenMethod(
            Fn<pressure_t(double)> getSize,
            Fn<AudioPosition()> getTime,
            WColor &color, WPen &pen
        ) : InsertTools(
                    std::move(getTime),
                    std::move(getSize),
                    color,
                    pen
                )
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
