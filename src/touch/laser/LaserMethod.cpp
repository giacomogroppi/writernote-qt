#include "LaserMethod.h"

#include <utility>
#include "core/core.h"
#include "touch/laser/Laser.h"
#include <functional>

extern StrokePre __tmp;

LaserMethod::LaserMethod(std::function<pressure_t(double)> getPress,
                         std::function<void()> objectMove,
                         std::function<void(const StrokePre &stroke)> append_to,
                         QPen &pen,
                         QColor &color)
    : InsertTools([]() {
        return 0;
    }
    , std::move(getPress)
    , std::move(objectMove)
    , color
    , pen)
    , _append_to(append_to)
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
    if (un(__tmp.isEmpty()))
        return -1;

    this->_append_to(__tmp);
    __tmp = StrokePre();

    return -1;
}

uchar LaserMethod::getAlfa() const
{
    return 255;
}
