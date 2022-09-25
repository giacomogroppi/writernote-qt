#pragma once

class frompdf;
class datastruct;
#include <QPointF>
#include <QSize>
#include "utils/common_def.h"

class zoom_control
{
private:
    void checkRespositioning(const QPointF &point, cbool max);
    datastruct *data;
public:
    bool zoom(const QPointF &pointMiddle, double delta, bool &zoomChange,
              const QSize &sizePixmap, const QSize &maxSize, datastruct *data);
};

