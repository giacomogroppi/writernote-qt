#pragma once

#include "utils/common_script.h"
#include <QTimer>
#include <QObject>
#include "testing/memtest.h"
#include "touch/datastruct/datastruct.h"
#include "touch/tabletcanvas.h"

class laser : public QObject
{
    Q_OBJECT
private:
    static constexpr int _size = 1;
    static constexpr int _time = 2000;

    QList<stroke> _stroke;

    QTimer *_timer;
    class TabletCanvas *_canvas;
public:
    laser(QObject *parent);
    ~laser();

    void startMove();
    void endMove();
    void append(const stroke &stroke);
    int length() const;
    const stroke &at(int i) const;

private slots:
    void endTimer();

};

force_inline const stroke &laser::at(int i) const
{
    return _stroke.at(i);
}

force_inline int laser::length() const
{
    return _stroke.length();
}

force_inline void laser::append(const stroke &stroke)
{
    _stroke.append(stroke);
}

force_inline void laser::endTimer()
{
    _stroke.clear();
    _canvas->call_update();
}

force_inline void laser::startMove()
{
    _timer->stop();
}

force_inline void laser::endMove()
{
    _timer->start(_time);
}

