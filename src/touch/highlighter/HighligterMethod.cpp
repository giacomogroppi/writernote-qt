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
    pressure_t pressure;
    StrokePre &strokeTmp = __tmp;

    const uchar alfa = 255;

    strokeTmp.setTime(_getTime());
    strokeTmp.setColor(_color);
    strokeTmp.setAlfaColor(alfa);

    pressure = this->_getSize(size);

    strokeTmp.append(point, pressure, _pen, getProp(doc));
}

bool HighligterMethod::touchUpdate(const QPointF &point, double size, Document &doc)
{
    StrokePre &strokeTmp = __tmp;
    pressure_t pressure;

    pressure = this->_getSize(size);

    strokeTmp.append(point, pressure, (QPen &)_pen, getProp(doc));

    core::get_canvas()->_finder->move(point);
}

int HighligterMethod::touchEnd(const QPointF &point, Document &doc)
{
    return 0;
}

uchar HighligterMethod::getAlfa() const
{
    return this->;
}
