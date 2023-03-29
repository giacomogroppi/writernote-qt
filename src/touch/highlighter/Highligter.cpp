#include "Highligter.h"

#include <utility>

Highligter::Highligter(QObject *parent,
                       std::function<int()> getTime,
                       std::function<void()> objectMove,
                       QColor &color,
                       QPen &pen)
    : QObject(parent)
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

void Highligter::setHighligterPressureConst()
{
    this->_type = HighligterType::HighligterTypeConstSize;
    emit onTypeChanged();
}

void Highligter::setHighligterSize()
{
    this->_type = HighligterType::HighligterTypePressure;
    emit onTypeChanged();
}

pressure_t Highligter::getSize(double pressure)
{
    static_assert(sizeof(pressure_t) <= sizeof(pressure));
    if (this->_type == HighligterType::HighligterTypeConstSize) {
        return this->_size;
    }
    return static_cast<pressure_t>(pressure) * 20.;
}

uchar Highligter::getAlfa() const
{
    return this->_alfa;
}

void Highligter::setAlfa(int alfa)
{
    W_ASSERT(alfa > 0 and alfa <= 255);
    this->_alfa = alfa;
    emit onAlfaChanged();
}

void Highligter::setSize(double size)
{
    this->_size = static_cast<pressure_t>(size);
    emit onSizeChanged();
}
