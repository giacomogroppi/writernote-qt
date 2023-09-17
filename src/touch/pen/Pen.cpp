#include "Pen.h"

#include <utility>

Pen::Pen(
            WObject *parent,
            Fn<int ()> getTime,
            WColor &color,
            WPen &pen
        )
    : WObject(parent)
    , PenMethod(
        [&](double press) -> pressure_t {
            return Pen::getSize(press);
        }
    , std::move(getTime)
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
