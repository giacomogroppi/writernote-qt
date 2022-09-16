#pragma once

class frompdf;
class datastruct;
#include <QPointF>
#include <QSize>

class zoom_control
{
private:
    void checkRespositioning(const QPointF &point, const bool max);
    datastruct *data;
public:
    bool zoom(const QPointF &pointMiddle, double delta, bool &zoomChange,
              const QSize &sizePixmap, const QSize &maxSize, datastruct *data);
};

