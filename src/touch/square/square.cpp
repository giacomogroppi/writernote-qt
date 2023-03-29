#include "square.h"
#include <QPen>
#include <QPainter>
#include <QDebug>

square::square(QObject *parent,
               std::function<void()> hideProperty,
               std::function<void(const QPointF& point, ActionProperty signal)> showProperty,
               std::function<Document &()> getDoc)
    : QObject(parent)
    , SquareMethod(hideProperty, showProperty, getDoc)
{
}

square::~square() = default;

void square::reset()
{
    SquareMethod::reset();
}

void square::needRefreshPrivate()
{
    emit this->needRefresh();
}
