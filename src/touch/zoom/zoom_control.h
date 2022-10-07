#pragma once

class frompdf;
class Document;
#include <QPointF>
#include <QSize>
#include "utils/common_def.h"

class zoom_control
{
private:
    void checkRespositioning(const QPointF &point, cbool max);
    Document *data;
public:
    bool zoom(const QPointF &pointMiddle, double delta, bool &zoomChange,
              const QSize &sizePixmap, const QSize &maxSize, Document *data);
};

