#pragma once

#include "touch/tools/Tools.h"

class SquareMethod: public Tools {
private:
    bool touchBeginPrivate(const QPointF& point, double size, class Document &doc);
    bool touchUpdatePrivate(const QPointF &point, double size, class Document &doc);
    int touchEndPrivate(const QPointF& point, class Document &doc);

public:
    SquareMethod();

    bool touchBegin(const QPointF& point, double size, class Document &doc) final;
    bool touchUpdate(const QPointF& point, double size, class Document &doc) final;
    int touchEnd(const QPointF& point, class Document &doc) final;
};


