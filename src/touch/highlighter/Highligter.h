#pragma once

#include "Scheduler/WObject.h"
#include "HighligterMethod.h"

class Highligter final:
        public WObject,
        public HighligterMethod
{
private:
    enum HighligterType {
        HighligterTypePressure,
        HighligterTypeConstSize
    };

    HighligterType _type;
    pressure_t _size;
    int _alfa;

    [[nodiscard]] unsigned char getAlfa() const override;
public:
    explicit Highligter(WObject *parent,
                        std::function<int()> getTime,
                        std::function<void(const PointF &)> objectMove,
                        WColor &color,
                        WPen &pen);

    ~Highligter() final = default;

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


inline constexpr int Highligter::type()
{
    return 2;
}
