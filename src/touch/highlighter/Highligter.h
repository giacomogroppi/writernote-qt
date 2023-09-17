#pragma once

#include "Scheduler/WObject.h"
#include "HighligterMethod.h"

class Highlighter final:
        public WObject,
        public HighligterMethod
{
private:
    enum HighlighterType {
        HighlighterTypePressure,
        HighlighterTypeConstSize
    };

    HighlighterType _type;
    pressure_t _size;
    int _alfa;

    [[nodiscard]] unsigned char getAlfa() const override;
public:
    explicit Highlighter(WObject *parent,
                         Fn<int()> getTime,
                         WColor &color,
                         WPen &pen);

    ~Highlighter() final = default;

    int getType() const final;
    static constexpr int type();

    DEFINE_LISTENER(setHighligterPressureConst());
    DEFINE_LISTENER(setHighligterSize());
    DEFINE_LISTENER(setSize(double size));
    DEFINE_LISTENER(setAlfa(int alfa));

public:
    pressure_t getSize(double pressure);

    W_EMITTABLE_0(onSizeChanged);
    W_EMITTABLE_0(onTypeChanged);
    W_EMITTABLE_0(onAlfaChanged);
};


inline constexpr int Highlighter::type()
{
    return 2;
}
