#pragma once

#include "testing/memtest.h"
#include "touch/dataTouch/datastruct/DataStruct.h"
#include "core/WList.h"
#include "utils/WCommonScript.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "LaserMethod.h"
#include "touch/pen/Pen.h"
#include "Scheduler/WTimer.h"

class Laser : public WObject,
              public LaserMethod
{
private:
    static constexpr int _size = 1;
    static constexpr int _time = 2000;

    WList<StrokePre> _stroke;
    Fn<void()> _callUpdate;
    WTimer *_timer;

public:
    explicit Laser(WObject *parent,
                   Fn<pressure_t(double)> getSize,
                   WColor &color, WPen &pen,
                   Fn<void()> callUpdate);
    ~Laser();

    void startMove();
    void endMove();
    void append(const StrokePre &stroke);
    [[nodiscard]] int length() const;
    [[nodiscard]] auto begin() { return this->_stroke.begin(); };
    [[nodiscard]] auto end() { return this->_stroke.end(); };

    int getType() const final;
    static constexpr int type();

private:
    void endTimer();
};

force_inline int Laser::length() const
{
    return _stroke.size();
}

force_inline void Laser::append(const StrokePre &stroke)
{
    _stroke.append(stroke);
}

force_inline void Laser::endTimer()
{
    _stroke.clear();
    _callUpdate();
}

force_inline void Laser::startMove()
{
    _timer->stop();
}

force_inline void Laser::endMove()
{
    _timer->start(_time);
}

inline constexpr int Laser::type()
{
    return 1;
}
