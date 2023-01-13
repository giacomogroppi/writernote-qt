#include "PenMethod.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "touch/tabletcanvas.h"
#include "touch/object_finder/object_finder.h"
#include <utility>

PenMethod::PenMethod(std::function<pressure_t(double)> getSize,
                     std::function<int()> getTime,
                     QColor &color, QPen &pen) :
        InsertTools(std::move(getTime),
                    std::move(getSize),
                    color,
                    pen)
{
}

bool PenMethod::touchBegin(const QPointF &point, double size, Document &doc)
{
    InsertTools::touchBegin(point, size, doc);
}

bool PenMethod::touchUpdate(const QPointF &point, double size, Document &doc)
{
    InsertTools::touchUpdate(point, size, doc);
}

int PenMethod::touchEnd(const QPointF &point, Document &doc)
{
    InsertTools::touchEnd(point, doc);
}

uchar PenMethod::getAlfa() const
{
    return 255;
}
