#include "Pen.h"

#include <utility>

using namespace std::placeholders;

Pen::Pen(
            WObject *parent,
            Fn<int ()> getTime,
            WColor &color,
            WPen &pen
        )
    : WObject(parent)
    , PenMethod(
                std::bind(&Pen::getSize, this, _1),
                std::move(getTime),
                color,
                pen
            )
    , _pressure(1.0)
    , _type(TypePen::TypeNoPressure)
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
