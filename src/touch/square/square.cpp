#include "square.h"
#include <QPen>
#include <QPainter>
#include <QDebug>
#include "touch/property/property_control.h"

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
