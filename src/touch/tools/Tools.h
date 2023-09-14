#pragma once

#include "utils/WCommonScript.h"
#include "currenttitle/document.h"
#include "touch/UpdateEvent.h"

class Tools {
private:
public:
    Tools() = default;
    ~Tools() = default;

    /**
     * @return the index of the page mod or -1 if not page has been modified
    */
    virtual auto touchBegin(const PointF& point, double size, class Document &doc) -> UpdateEvent = 0;

    /**
     * @return the index of the page mod or -1 if not page has been modified
    */
    virtual auto touchUpdate(const PointF& point, double size, class Document &doc) -> UpdateEvent = 0;

    /**
     * @return -1 if no page is mod
     * @return -2 if multiple pages have changed
     * @return the index of the page mod
    */
    virtual auto touchEnd(const PointF& point, class Document &doc) -> UpdateEvent = 0;
    virtual auto getType() const -> int= 0;
    static auto getProp(const Document &doc) -> double;
};

inline auto Tools::getProp(const Document &doc) -> double
{
    const auto prop = doc.getZoom() == PROP_RESOLUTION ?
            doc.getZoom() :
            doc.getZoom() - .0000001;
    return prop;
}
