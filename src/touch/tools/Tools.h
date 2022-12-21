#pragma once

#include "utils/WCommonScript.h"

class Tools {
private:
public:
    Tools() = default;
    ~Tools() = default;

    virtual bool touchBegin(const QPointF& point, class Document &doc) = 0;
    virtual bool touchUpdate(const QPointF& point, class Document &doc) = 0;

    /**
     * return -1 if no page is mod
     * return -2 if multiple pages have changed
     * return the index of the page mod
    */
    virtual int touchEnd(const QPointF& point, class Document &doc) = 0;
};
