#include "square.h"
#include <QPen>
#include <QPainter>
#include "currenttitle/document.h"
#include <QDebug>
#include "images/fromimage.h"
#include "log/log_ui/log_ui.h"
#include "touch/property/property_control.h"
#include "touch/tabletcanvas.h"
#include "utils/WCommonScript.h"
#include "touch/multi_thread_data.h"
#include "touch/paintevent/paint.h"
#include "pthread.h"
#include "testing/memtest.h"

square::square(QObject *parent, property_control *property)
    : QObject(parent)
    , SquareMethod(property)
{
    QObject::connect(property, &property_control::ActionSelection,
                     [&](property_control::ActionProperty action) {
        SquareMethod::actionProperty(action);
    });
}

square::~square()
= default;



void square::reset()
{
    SquareMethod::reset();
}
