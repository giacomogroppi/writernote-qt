#include "Laser.h"

#include <utility>
#include "touch/TabletUtils.h"

Laser::Laser(QObject *parent,
             std::function<pressure_t(double)> getSize,
             std::function<void(const QPointF&)> objectMove,
             QColor &color,
             QPen &pen,
             std::function<void()> callUpdate)
    : QObject(parent)
    , LaserMethod(
        std::move(getSize),
        std::move(objectMove),
        [this](const StrokePre &stroke) {
            this->append(stroke);
        },
        pen,
        color)
    , _callUpdate(std::move(callUpdate))
    , _timer(new QTimer())
{
    QObject::connect(_timer, &QTimer::timeout, this, &Laser::endTimer);
}

Laser::~Laser()
{
    delete this->_timer;
}
