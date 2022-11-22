#pragma once

#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "touch/dataTouch/point.h"
#include <QPainter>
#include <QPen>
#include "core/WList.h"
#include "StrokeNormal.h"

class StrokeForPage {
private:
    StrokeNormal _data;
public:
    StrokeForPage();
    ~StrokeForPage() = default;
    void setMetadata(const colore_s &colore);
    void append(const point_s &point, pressure_t pressure);

    void reset();
    friend class Page;
};

inline void StrokeForPage::append(const point_s &point, pressure_t pressure)
{
    _data.append(point, pressure);
}

inline void StrokeForPage::reset()
{
    _data.reset();
}

inline void StrokeForPage::setMetadata(const colore_s &colore)
{
    _data.setMetadata(-1, colore);
}

