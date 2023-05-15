#include "Square.h"
#include "core/WPen.h"
#include "core/WPainter/WPainter.h"
#include <QDebug>

Square::Square(WObject *parent,
               std::function<void()> hideProperty,
               std::function<void(const PointF& point, ActionProperty signal)> showProperty,
               std::function<Document &()> getDoc, std::function<void ()> callUpdate)
    : WObject(parent)
    , SquareMethod(hideProperty, showProperty, getDoc)
    , _callUpdate(callUpdate)
{
}

int Square::getType() const
{
    return Square::type();
}

Square::~Square() = default;

void Square::reset()
{
    SquareMethod::reset();
}

void Square::needRefreshPrivate()
{
    _callUpdate();
}
