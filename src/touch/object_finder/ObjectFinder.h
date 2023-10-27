#pragma once

#include "utils/WCommonScript.h"
#include "touch/dataTouch/Point.h"
#include "Scheduler/WObject.h"
#include "Scheduler/WTimer.h"

/*
class ObjectFinder : public WObject
{
public:
    explicit ObjectFinder(WObject *parent, Fn<void()> callUpdate);
    ~ObjectFinder() override = default;

    void move(const PointF &point);
    void endMoving();
    void reset();

private:
    Fn<void()> _callUpdate;
    [[nodiscard]] bool isActive() const;
    PointSettable _point;
    static constexpr auto debug = false;
    static constexpr auto time = 1 * 500;
    WTimer *_timer;

    DEFINE_LISTENER(endTimer());
};

force_inline void ObjectFinder::endMoving()
{
    return this->reset();
}

force_inline void ObjectFinder::move(const PointF& point)
{
    WDebug(debug, "ObjectFinder");

    if(_point.isSet()){
        // if the point is equal we don't have to stop the timer
        if(un(WUtils::is_near(_point, point, 1.))){
            return;
        }

        // if the point is differente we neet to write it and restart the timer
        _point = point;
    }else{
        _point.set(true);
        _point = point;
    }

    if(_timer->isActive()){
        _timer->stop();
    }

    _timer->start(time);
}

force_inline void ObjectFinder::reset()
{
    _point.set(false);
    WDebug(debug, "ObjectFinder" << __FUNCTION__);
    if(isActive()){
        _timer->stop();
    }
}

force_inline bool ObjectFinder::isActive() const
{
    return _timer->isActive();
}
*/
