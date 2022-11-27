#pragma once

#include "Stroke.h"

class StrokeCircle final: public Stroke
{
private:
    double _r, _x, _y;
public:
    StrokeCircle();
    QRect getBiggerPointInStroke() const final;

    void adjust(double zoom) final;
};

