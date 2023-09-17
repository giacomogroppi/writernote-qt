#include "Laser.h"

#include <utility>
#include "touch/TabletUtils.h"

Laser::Laser(WObject *parent,
             Fn<pressure_t(double)> getSize,
             WColor &color,
             WPen &pen,
             Fn<void()> callUpdate)
    : WObject(parent)
    , LaserMethod(
                std::move(getSize),
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
