#pragma once

#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "touch/dataTouch/point.h"
#include <QPainter>
#include <QPen>
#include "core/WList.h"
#include "StrokeNormal.h"

class StrokeForPage: private StrokeNormal {
private:
    WList<point_s> _point;
    pressure_t _press;
    colore_s _color;

public:
    StrokeForPage();
    ~StrokeForPage() = default;
    void setMetadata(int pos, const colore_s &colore);
};

inline void StrokeForPage::setMetadata(int pos, const colore_s &colore)
{
    StrokeNormal::setMetadata(pos, colore);
}

