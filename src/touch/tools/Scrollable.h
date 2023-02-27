#pragma once

#include <QPointF>

class Scrollable {
public:
    virtual void translate(const QPointF &point) = 0;
};


