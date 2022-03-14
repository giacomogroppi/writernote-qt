#ifndef OBJECT_FINDER_H
#define OBJECT_FINDER_H

#include "utils/common_script.h"
#include "touch/datastruct/point.h"
#include <QObject>
#include <QTimer>
#include <QPointF>

class object_finder : public QObject
{
    Q_OBJECT
public:
    explicit object_finder(QObject *parent);
    ~object_finder();

    void move(const QPointF &point);
    void endMoving();
    void reset();

private:
    bool isActive() const;
    PointSettable _point;
    static constexpr auto debug = false;
    static constexpr auto time = 1 * 500;
    class TabletCanvas *_canvas;
    QTimer *_timer;

private slots:
    void endTimer();

};

force_inline void object_finder::endMoving()
{
    return this->reset();
}

force_inline void object_finder::move(const QPointF& point)
{
    WDebug(debug, "object_finder" << __FUNCTION__);

    if(likely(_point.set)){
        // if the point is equal we don't have to stop the timer
        if(unlikely(is_near(_point.point, point, 1.))){
            return;
        }

        // if the point is differente we neet to save it and restart the timer
        _point.point = point;
    }else{
        _point.set = true;
        _point.point = point;
    }

    if(likely(_timer->isActive())){
        _timer->stop();
    }

    _timer->start(time);
}

force_inline void object_finder::reset()
{
    _point.set = false;
    WDebug(debug, "object_finder" << __FUNCTION__);
    if(isActive()){
        _timer->stop();
    }
}

force_inline bool object_finder::isActive() const
{
    return _timer->isActive();
}

#endif // OBJECT_FINDER_H
