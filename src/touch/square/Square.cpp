#include "Square.h"

#include <utility>
#include "core/WPen.h"
#include "core/WPainter/WPainter.h"

Square::Square(WObject *parent,
               std::function<void()> hideProperty,
               std::function<void(const PointF& point, ActionProperty signal)> showProperty,
               std::function<Document &()> getDoc, std::function<void ()> callUpdate)
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

void Square::needRefreshPrivate()
{
    _callUpdate();
}
