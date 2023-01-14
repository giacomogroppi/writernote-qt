#include "laser.h"
#include "touch/tabletcanvas.h"

laser::laser(QObject *parent,
             std::function<pressure_t(double)> getSize,
             QColor &_color,
             QPen &_pen):
    QObject(parent),
    LaserMethod(getSize, _pen, _color)
{
    WNew(_timer, QTimer, ());
    QObject::connect(_timer, &QTimer::timeout, this, &laser::endTimer);
    _canvas = (TabletCanvas *)parent;

    W_ASSERT(_canvas->objectName() == "TabletCanvas");

}

laser::~laser()
{
    WDelete(_timer);
}
