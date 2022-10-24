#pragma once

#include "utils/common_script.h"
#include "touch/dataTouch/point.h"

#include <QPainter>

class StrokeComplex {
public:
    virtual void draw(QPainter &painter, QPen &pen, double zoom);
    virtual void save();
};
