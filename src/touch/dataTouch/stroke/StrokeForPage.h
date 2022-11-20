#pragma once

#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "touch/dataTouch/point.h"
#include <QPainter>
#include <QPen>
#include "core/WList.h"

class StrokeForPage {
private:
    WList<point_s> _point;
    pressure_t _press;
public:
    StrokeForPage();
    ~StrokeForPage() = default;
    void draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const;
};

