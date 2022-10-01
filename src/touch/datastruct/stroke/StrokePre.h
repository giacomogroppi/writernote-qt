#pragma once

#include "Stroke.h"
#include "core/WList.h"
#include "core/WImage.h"

class StrokePre: private Stroke, private WImage {
private:
    WList<point_s> _point;
public:
    StrokePre();
    ~StrokePre() = default;

    void reset();

};
