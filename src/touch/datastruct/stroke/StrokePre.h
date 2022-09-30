#pragma once

#include "Stroke.h"
#include "core/WList.h"
#include "core/WImage.h"

class StrokePre: public Stroke, private WImage {
private:
    WList<point_s> _p;
public:
    StrokePre();
    ~StrokePre() = default;
};
