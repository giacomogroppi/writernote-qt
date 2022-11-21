#pragma once

#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "touch/dataTouch/point.h"
#include <QPainter>
#include <QPen>
#include "core/WList.h"
#include "StrokeNormal.h"

class StrokeForPage: private StrokeNormal {
private:
public:
    StrokeForPage();
    ~StrokeForPage() override = default;
    void setMetadata(const colore_s &colore);

    void reset() final;
    friend class Page;
};

inline void StrokeForPage::reset()
{
    StrokeNormal::reset();
}

inline void StrokeForPage::setMetadata(const colore_s &colore)
{
    StrokeNormal::setMetadata(-1, colore);
}

