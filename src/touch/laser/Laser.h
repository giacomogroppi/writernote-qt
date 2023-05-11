#pragma once

#include "testing/memtest.h"
#include "touch/dataTouch/datastruct/DataStruct.h"
#include "core/WList.h"
#include "utils/WCommonScript.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "LaserMethod.h"
#include "touch/pen/Pen.h"

class Laser : public WObject,
              public LaserMethod
{
private:
    static constexpr int _size = 1;
    static constexpr int _time = 2000;

    WList<StrokePre> _stroke;
    std::function<void()> _callUpdate;
    QTimer *_timer;

public:
    explicit Laser(WObject *parent,
                   std::function<pressure_t(double)> getSize,
                   std::function<void(const PointF&)> objectMove,
                   colore_s &color, WPen &pen,
                   std::function<void()> callUpdate);
    ~Laser();

    void startMove();
    void endMove();
    void append(const StrokePre &stroke);
    [[nodiscard]] int length() const;
    [[nodiscard]] auto begin() { return this->_stroke.begin(); };
    [[nodiscard]] auto end() { return this->_stroke.end(); };

    int getType() const final;
    static constexpr int type();
private slots:
    void endTimer();

};

force_inline int Laser::length() const
{
    return _stroke.length();
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
