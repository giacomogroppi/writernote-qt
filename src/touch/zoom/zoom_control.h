#ifndef ZOOM_CONTROL_H
#define ZOOM_CONTROL_H

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
    bool zoom(QPointF &,
              double,
              const uint widthP,
              const uint maxWidth,
              const uint maxHeight,
              const uint height,
              datastruct *data);

};

#endif // ZOOM_CONTROL_H
