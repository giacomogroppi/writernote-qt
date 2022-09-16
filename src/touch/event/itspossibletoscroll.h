#pragma once

#include <QSize>
#include <QPointF>
#include "src/utils/common_def.h"

class datastruct;

namespace scroll {
    bool y(const datastruct *data, cint height, double &__pos_delta);
    bool x(const datastruct *data, cint width, double &__pos_delta);
    bool itspossibletoscroll(const datastruct &data, const QSize &size, QPointF &__pos_delta);
};

