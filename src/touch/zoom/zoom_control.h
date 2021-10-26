#ifndef ZOOM_CONTROL_H
#define ZOOM_CONTROL_H

class frompdf;
class datastruct;
#include <QPointF>

class zoom_control
{
private:
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
