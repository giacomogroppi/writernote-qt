#pragma once

#include <QTimer>
#include <QObject>
#include "testing/memtest.h"
#include "touch/dataTouch/datastruct/datastruct.h"
#include "touch/tabletcanvas.h"
#include "core/WList.h"
#include "utils/WCommonScript.h"
#include "touch/dataTouch/stroke/StrokePre.h"

class laser :   public QObject,
                public LaserMethod
{
    Q_OBJECT
private:
    static constexpr int _size = 1;
    static constexpr int _time = 2000;

    WList<StrokePre> _stroke;

    QTimer *_timer;
    class TabletCanvas *_canvas;

public:
    explicit laser(QObject *parent, std::function<pressure_t(double)> getSize,
                   QColor &_color, QPen &_pen);
    ~laser();

    void startMove();
    void endMove();
    void append(const StrokePre &stroke);
    [[nodiscard]] int length() const;
    [[nodiscard]] auto begin();
    [[nodiscard]] auto end();

private slots:
    void endTimer();

};

inline auto laser::begin()
{
    return _stroke.begin();
}

inline auto laser::end()
{
    return _stroke.end();
}

force_inline int laser::length() const
{
    return _stroke.length();
}

force_inline void laser::append(const StrokePre &stroke)
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

