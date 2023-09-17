#include "Square.h"

#include <utility>
#include "core/Pen/WPen.h"
#include "core/WPainter/WPainter.h"

Square::Square(WObject *parent,
               Fn<void()> hideProperty,
               Fn<void(const PointF& point, ActionProperty signal)> showProperty,
               Fn<Document &()> getDoc, Fn<void (UpdateEvent event)> callUpdate)
    : WObject(parent)
    , SquareMethod(std::move(hideProperty), std::move(showProperty), std::move(getDoc))
    , _callUpdate(std::move(callUpdate))
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

void Square::needRefreshPrivate(UpdateEvent event)
{
    _callUpdate(event);
}
