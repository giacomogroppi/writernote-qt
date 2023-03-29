#pragma once

#include "utils/WCommonScript.h"
#include "touch/dataTouch/Point.h"
#include <QObject>
#include <QTimer>
#include <QPointF>

class ObjectFinder : public QObject
{
    Q_OBJECT
public:
    explicit ObjectFinder(QObject *parent, std::function<void()> callUpdate);
    ~ObjectFinder() override = default;

    void move(const QPointF &point);
    void endMoving();
    void reset();

private:
    std::function<void()> _callUpdate;
    [[nodiscard]] bool isActive() const;
    PointSettable _point;
    static constexpr auto debug = false;
    static constexpr auto time = 1 * 500;
    QTimer *_timer;

    Q_DISABLE_COPY(ObjectFinder);

private slots:
    void endTimer();
};

force_inline void ObjectFinder::endMoving()
{
    return this->reset();
}

force_inline void ObjectFinder::move(const QPointF& point)
{
    WDebug(debug, "ObjectFinder" << __FUNCTION__);

    if(likely(_point.isSet())){
        // if the point is equal we don't have to stop the timer
        if(un(WCommonScript::is_near(_point, point, 1.))){
            return;
        }

        // if the point is differente we neet to save it and restart the timer
        _point = point;
    }else{
        _point.setSet(true);
        _point = point;
    }

    if(likely(_timer->isActive())){
        _timer->stop();
    }

    _timer->start(time);
}

force_inline void ObjectFinder::reset()
{
    _point.setSet(false);
    WDebug(debug, "ObjectFinder" << __FUNCTION__);
    if(isActive()){
        _timer->stop();
    }
}

force_inline bool ObjectFinder::isActive() const
{
    return _timer->isActive();
}

