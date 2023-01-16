#pragma once

#include "utils/WCommonScript.h"
#include "currenttitle/document.h"

class Tools {
private:
public:
    Tools() = default;
    ~Tools() = default;

    virtual bool touchBegin(const QPointF& point, double size, class Document &doc) = 0;
    virtual bool touchUpdate(const QPointF& point, double size, class Document &doc) = 0;

    /**
     * return -1 if no page is mod
     * return -2 if multiple pages have changed
     * return the index of the page mod
    */
    virtual int touchEnd(const QPointF& point, class Document &doc) = 0;

    static double getProp(const Document &doc);
};

inline double Tools::getProp(const Document &doc)
{
    const auto prop = doc.getZoom() == PROP_RESOLUTION ?
            doc.getZoom() :
            doc.getZoom() - .0000001;
    return prop;
}