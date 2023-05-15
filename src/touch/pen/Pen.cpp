#include "Pen.h"

#include <utility>

Pen::Pen(WObject *parent,
         std::function<int ()> getTime,
         std::function<void(const PointF &)> objectMove,
         colore_s &color,
         WPen &pen)
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

int Pen::getType() const
{
    return Pen::type();
}

pressure_t Pen::getSize(double press) const
{
    if (this->isPressureVariable())
        return press;
    return this->_pressure;
}
