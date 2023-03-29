#include "Square.h"
#include <QPen>
#include <QPainter>
#include <QDebug>

Square::Square(QObject *parent,
               std::function<void()> hideProperty,
               std::function<void(const QPointF& point, ActionProperty signal)> showProperty,
               std::function<Document &()> getDoc)
    : QObject(parent)
    , SquareMethod(hideProperty, showProperty, getDoc)
{
}

Square::~Square() = default;

void Square::reset()
{
    SquareMethod::reset();
}

void Square::needRefreshPrivate()
{
    emit this->needRefresh();
}
