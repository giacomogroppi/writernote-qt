#include "LaserMethod.h"

#include <utility>
#include "core/core.h"
#include "touch/laser/Laser.h"
#include <functional>

extern StrokePre *__tmp;

LaserMethod::LaserMethod(std::function<pressure_t(double)> getPress,
                         std::function<void(const PointF&)> objectMove,
                         std::function<void(const StrokePre &stroke)> append_to,
                         WPen &pen,
                         WColor &color)
    : InsertTools([]() {
        return 0;
    }
    , std::move(getPress)
    , std::move(objectMove)
    , color
    , pen)
    , _append_to(std::move(append_to))
{
}

auto LaserMethod::touchBegin(const PointF &point, double size, Document &doc) -> UpdateEvent
{
    return InsertTools::touchBegin(point, size, doc);
}

auto LaserMethod::touchUpdate(const PointF &point, double size, Document &doc) -> UpdateEvent
{
    return InsertTools::touchUpdate(point, size, doc);
}

auto LaserMethod::touchEnd(const PointF &, Document &) -> UpdateEvent
{
    if (un(__tmp->isEmpty()))
        return UpdateEvent::makeEmpty();

    this->_append_to(*__tmp);
    *__tmp = StrokePre();

    return UpdateEvent::makeEmpty();
}

unsigned char LaserMethod::getAlfa() const
{
    return 255;
}
