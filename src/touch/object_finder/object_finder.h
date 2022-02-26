#ifndef OBJECT_FINDER_H
#define OBJECT_FINDER_H

#include "utils/common_script.h"

#include <QObject>
#include <QTimer>
#include <QPointF>

class object_finder : public QObject
{
    Q_OBJECT
public:
    explicit object_finder(QObject *parent);
    ~object_finder();

    void move();
    void endMoving();
    void reset();

private:
    bool isActive() const;

    static constexpr auto debug = false;
    static constexpr auto time = 1 * 1000;
    class TabletCanvas *_canvas;
    QTimer *_timer;

private slots:
    void endTimer();

};

force_inline void object_finder::endMoving()
{
    return this->reset();
}

force_inline void object_finder::move()
{
    WDebug(debug, "object_finder" << __FUNCTION__);
    if(likely(_timer->isActive())){
        _timer->stop();
    }

    _timer->start(time);
}

force_inline void object_finder::reset()
{
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
