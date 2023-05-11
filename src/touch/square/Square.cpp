#include "Square.h"
#include <QPen>
#include <QPainter>
#include <QDebug>

Square::Square(QObject *parent,
               std::function<void()> hideProperty,
               std::function<void(const QPointF& point, ActionProperty signal)> showProperty,
               std::function<Document &()> getDoc, std::function<void ()> callUpdate)
    : QObject(parent)
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
