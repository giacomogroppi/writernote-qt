#pragma once

#include "core/PointF.h"

class Scrollable {
public:
    virtual void translate(const PointF &point) = 0;
};


