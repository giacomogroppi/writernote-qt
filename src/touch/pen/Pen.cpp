#include "Pen.h"

#include <utility>

Pen::Pen(QObject *parent,
         std::function<int ()> getTime,
         std::function<void()> objectMove,
         QColor &color,
         QPen &pen)
    : PenMethod(
        [&](double press) -> pressure_t {
            return Pen::getSize(press);
        }
    , std::move(getTime)
    , std::move(objectMove)
    , color
    , pen)
{
    // TODO: load from QSettings
    this->_pressure = 5.;
}

void Pen::setType(TypePen type)
{
    this->_type = type;
}

pressure_t Pen::getSize(double press) const
{
    if (this->isPressureVariable())
        return press;
    return this->_pressure;
}
