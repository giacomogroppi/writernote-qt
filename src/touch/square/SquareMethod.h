#pragma once

#include "touch/tools/Tools.h"

class SquareMethod: public Tools {
private:
public:
    SquareMethod();

    bool touchBegin(const QPointF& point, double size, class Document &doc) final;
    bool touchUpdate(const QPointF& point, double size, class Document &doc) final;
    int touchEnd(const QPointF& point, class Document &doc) final;
};


