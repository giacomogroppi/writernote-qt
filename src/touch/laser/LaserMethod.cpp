#include "LaserMethod.h"

#include <utility>
#include "core/core.h"
#include "touch/laser/laser.h"

extern StrokePre __tmp;

LaserMethod::LaserMethod(std::function<pressure_t(double)> getPress,
                         QPen &pen,
                         QColor &color) :
        InsertTools([]() {return 0;},
                    std::move(getPress),
                    color,
                    pen)
{
}

bool LaserMethod::touchBegin(const QPointF &point, double size, Document &doc)
{
    return InsertTools::touchBegin(point, size, doc);
}

bool LaserMethod::touchUpdate(const QPointF &point, double size, Document &doc)
{
    return InsertTools::touchUpdate(point, size, doc);
}

int LaserMethod::touchEnd(const QPointF &, Document &)
{
    auto *canvas = core::get_canvas();

    if (un(__tmp.isEmpty()))
        return -1;

    canvas->_laser->append(__tmp);
    canvas->_laser->endMove();
    __tmp = StrokePre();

    return -1;
}

uchar LaserMethod::getAlfa() const
{
    return 255;
}
