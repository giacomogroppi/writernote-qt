#pragma once

#include "PenMethod.h"
#include "touch/dataTouch/datastruct/DataStruct.h"
#include "Scheduler/WObject.h"

class Pen final: public WObject, public PenMethod
{
public:
    Pen(WObject *parent,
        Fn<AudioPosition()> getTime,
        WColor &color, WPen &pen);
    ~Pen() final = default;

    enum TypePen {
        TypePressure,
        TypeNoPressure
    };

    void setType(enum TypePen type);

    nd auto getType() const -> int final;
    static constexpr int type();

private:
    double _pressure;
    nd auto getSize(double getPressure) const -> pressure_t;
    enum TypePen _type;

    nd auto isPressureVariable() const -> bool;
};

inline constexpr int Pen::type()
{
    return 0;
}

inline bool Pen::isPressureVariable() const
{
    return this->_type == TypePen::TypePressure;
}
