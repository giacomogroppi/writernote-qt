#include "laser.h"

laser::laser(QObject *parent):
    QObject(parent)
{
    WNew(_timer, QTimer, ());
    QObject::connect(_timer, &QTimer::timeout, this, &laser::endTimer);
}

laser::~laser()
{
    WDelete(_timer);
}
