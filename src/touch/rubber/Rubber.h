#pragma once

#include "currenttitle/document.h"
#include <pthread.h>
#include "RubberMethod.h"

#define POSITION_ALFA 3
#define DECREASE 2

class Rubber : public WObject,
               public RubberMethod
{
public:
    explicit Rubber(WObject *parent);
    virtual ~Rubber();

    int getType() const final;
    static constexpr int type();

    void setRubberTotal();
    void setRubberPartial();

    W_EMITTABLE_0(onRubberChange);

private:
    int _size_gomma = DEFAULT_GOMMA_SIZE;
    RubberMethod::type_rubber _type_gomma = RubberMethod::total;
};


inline constexpr int Rubber::type()
{
    return 3;
}
