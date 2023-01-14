#include "HighligterMethod.h"
#include "core/core.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "touch/tabletcanvas.h"
#include "touch/object_finder/object_finder.h"
#include <utility>

extern StrokePre __tmp;

HighligterMethod::HighligterMethod(
            std::function<int()> getTime,
            std::function<pressure_t(double)> getSize,
            QPen &pen,
            QColor &color
        ):
        InsertTools(std::move(getTime),
                    std::move(getSize),
                    color,
                    pen)
{
}

bool HighligterMethod::touchBegin(const QPointF &point, double size, Document &doc)
{
    InsertTools::touchBegin(point, size, doc);
}

bool HighligterMethod::touchUpdate(const QPointF &point, double size, Document &doc)
{
    InsertTools::touchUpdate(point, size, doc);
}

int HighligterMethod::touchEnd(const QPointF &point, Document &doc)
{
    InsertTools::touchEnd(point, doc);
}
