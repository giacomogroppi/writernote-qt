#include "HighligterMethod.h"
#include "core/core.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "touch/object_finder/ObjectFinder.h"
#include <utility>

extern StrokePre *__tmp;

HighligterMethod::HighligterMethod(
            std::function<int()> getTime,
            std::function<pressure_t(double)> getSize,
            std::function<void(const QPointF&)> objectMove,
            QPen &pen,
            QColor &color
        )
    : InsertTools(std::move(getTime),
                  std::move(getSize),
                  std::move(objectMove),
                  color,
                  pen)
{
}

int HighligterMethod::touchBegin(const QPointF &point, double size, Document &doc)
{
    return InsertTools::touchBegin(point, size, doc);
}

int HighligterMethod::touchUpdate(const QPointF &point, double size, Document &doc)
{
    return InsertTools::touchUpdate(point, size, doc);
}

int HighligterMethod::touchEnd(const QPointF &point, Document &doc)
{
    return InsertTools::touchEnd(point, doc);
}
