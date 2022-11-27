#pragma once

#include "Stroke.h"

class StrokeRect final: public Stroke
{
private:
    QRectF _rect;
public:
    StrokeRect();

    QRect getBiggerPointInStroke() const final;
    void adjust(double zoom) final;
};

