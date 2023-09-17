#pragma once

#include "PenMethod.h"
#include "touch/dataTouch/datastruct/DataStruct.h"
#include "Scheduler/WObject.h"

class Pen: public WObject, public PenMethod
{
public:
    Pen(WObject *parent,
        Fn<int()> getTime,
        WColor &color, WPen &pen);
    virtual ~Pen() = default;

    enum TypePen {
        TypePressure,
        TypeNoPressure
    };

    void setType(enum TypePen type);

    int getType() const final;
    static constexpr int type();

private:
    double _pressure;
    pressure_t getSize(double getPressure) const;
    enum TypePen _type;

    bool isPressureVariable() const;
};

inline constexpr int Pen::type()
{
    return 0;
}

inline bool Pen::isPressureVariable() const
{
    return this->_type == TypePen::TypePressure;
}
