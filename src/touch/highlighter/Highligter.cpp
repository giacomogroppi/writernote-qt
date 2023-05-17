#include "Highligter.h"

#include <utility>

Highligter::Highligter(WObject *parent,
                       std::function<int()> getTime,
                       std::function<void(const PointF &)> objectMove,
                       colore_s &color,
                       WPen &pen)
    : WObject(parent)
    , HighligterMethod(std::move(getTime),
                       [&](double press) -> pressure_t {
                            return Highligter::getSize(press);
                        },
                       std::move(objectMove),
                       pen,
                       color)
    , _type(HighligterType::HighligterTypeConstSize)
{
    // TODO: load data from settings
}

int Highligter::getType() const
{
    return Highligter::type();
}

void Highligter::setHighligterPressureConst()
{
    this->_type = HighligterType::HighligterTypeConstSize;
    W_EMIT_0(onTypeChanged);
}

void Highligter::setHighligterSize()
{
    this->_type = HighligterType::HighligterTypePressure;
    W_EMIT_0(onTypeChanged);
}

pressure_t Highligter::getSize(double pressure)
{
    static_assert(sizeof(pressure_t) <= sizeof(pressure));
    if (this->_type == HighligterType::HighligterTypeConstSize) {
        return this->_size;
    }
    return static_cast<pressure_t>(pressure) * 20.;
}

unsigned char Highligter::getAlfa() const
{
    return this->_alfa;
}

void Highligter::setAlfa(int alfa)
{
    W_ASSERT(alfa > 0 and alfa <= 255);
    this->_alfa = alfa;
    W_EMIT_0(onAlfaChanged);
}

void Highligter::setSize(double size)
{
    this->_size = static_cast<pressure_t>(size);
    W_EMIT_0(onSizeChanged);
}
