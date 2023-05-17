#include "Laser.h"

#include <utility>
#include "touch/TabletUtils.h"

Laser::Laser(WObject *parent,
             std::function<pressure_t(double)> getSize,
             std::function<void(const PointF&)> objectMove,
             colore_s &color,
             WPen &pen,
             std::function<void()> callUpdate)
    : WObject(parent)
    , LaserMethod(
        std::move(getSize),
        std::move(objectMove),
        [this](const StrokePre &stroke) {
            this->append(stroke);
        },
        pen,
        color)
    , _callUpdate(std::move(callUpdate))
    , _timer(new WTimer(this, [this]() { this->endTimer(); }, Laser::_time ))
{
}

Laser::~Laser()
{
    delete this->_timer;
}

int Laser::getType() const
{
    return Laser::type();
}
