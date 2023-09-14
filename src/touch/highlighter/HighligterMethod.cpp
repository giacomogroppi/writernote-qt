#include "HighligterMethod.h"
#include "core/core.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "touch/object_finder/ObjectFinder.h"
#include <utility>

extern StrokePre *__tmp;

HighligterMethod::HighligterMethod(
        std::function<int()> getTime,
        std::function<pressure_t(double)> getSize,
        std::function<void(const PointF&)> objectMove,
        WPen &pen,
        WColor &color
        )
    : InsertTools(std::move(getTime),
                  std::move(getSize),
                  std::move(objectMove),
                  color,
                  pen)
{
}

auto HighligterMethod::touchBegin(const PointF &point, double size, Document &doc) -> UpdateEvent
{
    return InsertTools::touchBegin(point, size, doc);
}

auto HighligterMethod::touchUpdate(const PointF &point, double size, Document &doc) -> UpdateEvent
{
    return InsertTools::touchUpdate(point, size, doc);
}

auto HighligterMethod::touchEnd(const PointF &point, Document &doc) -> UpdateEvent
{
    return InsertTools::touchEnd(point, doc);
}
