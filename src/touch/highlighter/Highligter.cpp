#include "Highligter.h"

#include <utility>

Highlighter::Highlighter(WObject *parent,
                         Fn<int()> getTime,
                         WColor &color,
                         WPen &pen)
    : WObject(parent)
    , HighligterMethod(std::move(getTime),
                       [&](double press) -> pressure_t {
                            return Highlighter::getSize(press);
                        },
                       pen,
                       color)
    , _type(HighlighterType::HighlighterTypeConstSize)
{
    // TODO: load data from settings
}

int Highlighter::getType() const
{
    return Highlighter::type();
}

void Highlighter::setHighligterPressureConst()
{
    this->_type = HighlighterType::HighlighterTypeConstSize;
    W_EMIT_0(onTypeChanged);
}

void Highlighter::setHighligterSize()
{
    this->_type = HighlighterType::HighlighterTypePressure;
    W_EMIT_0(onTypeChanged);
}

pressure_t Highlighter::getSize(double pressure)
{
    static_assert(sizeof(pressure_t) <= sizeof(pressure));
    if (this->_type == HighlighterType::HighlighterTypeConstSize) {
        return this->_size;
    }
    return static_cast<pressure_t>(pressure) * 20.;
}

unsigned char Highlighter::getAlfa() const
{
    return this->_alfa;
}

void Highlighter::setAlfa(int alfa)
{
    W_ASSERT(alfa > 0 and alfa <= 255);
    this->_alfa = alfa;
    W_EMIT_0(onAlfaChanged);
}

void Highlighter::setSize(double size)
{
    this->_size = static_cast<pressure_t>(size);
    W_EMIT_0(onSizeChanged);
}
