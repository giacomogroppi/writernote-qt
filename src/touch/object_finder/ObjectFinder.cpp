#include "ObjectFinder.h"

#include <utility>
#include "touch/object_finder/model_finder/model_finder.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "touch/TabletUtils.h"


// è lo stesso stroke definito nel file tabletevent.cpp
extern StrokePre *__tmp;

ObjectFinder::ObjectFinder(WObject *parent, std::function<void()> callUpdate)
    : WObject{parent}
    , _callUpdate(std::move(callUpdate))
    , _timer(new WTimer(this, [this]() { endTimer(); }, time))
{
    _timer->setSingleShot(true);
}


void ObjectFinder::endTimer()
{
    StrokePre &stroke = *__tmp;

    if (!stroke.isEmpty()) {
        WDebug(debug, "call");

        if (model::find(stroke)) {
            this->_callUpdate();
            stroke.reset_img();
        }
    }
}
