#include "object_finder.h"
#include "mainwindow.h"
#include "currenttitle/document.h"
#include "touch/object_finder/model_finder/model_finder.h"
#include "pthread.h"

// è lo stesso stroke definito nel file tabletevent.cpp
extern stroke __tmp;

object_finder::object_finder(QObject *parent)
    : QObject{parent}
{
    W_ASSERT(parent);
    W_ASSERT(parent->objectName() == "TabletCanvas");

    _canvas = (TabletCanvas *)parent;

    _timer = new QTimer(this);
    QObject::connect(_timer, &QTimer::timeout, this, &object_finder::endTimer);
    _timer->setSingleShot(true);
}

object_finder::~object_finder()
{

}

void object_finder::endTimer()
{
    stroke &stroke = __tmp;
    WDebug(debug, "object_finder" << __FUNCTION__);
    model::find(&stroke);
    _canvas->call_update();
}
