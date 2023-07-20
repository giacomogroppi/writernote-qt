#pragma once

#include <thread>
#include <chrono>
#include <iostream>
#include "WObject.h"
#include "core/AtomicSafe.h"
#include "core/WRecursiveLock.h"

class WTimer: public WObject
{
private:
    mutable WRecursiveLock _lock;
    bool _isActive;
    unsigned long _millisecond;
    unsigned long _timeStart;

    std::function<void()> _function;
public:
    WTimer(WObject *parent, std::function<void()> function, int millisecond);
    ~WTimer() override = default;

    bool isActive() const;
    void stop();
    void start(int millisecond = -1);
    void setSingleShot(bool singleShot);

    /**
     * \return the value of clock() when the timer should be triggered
     * */
    auto getEnd() const -> unsigned long;

    /**
     * \return The duration in milliseconds of the timer
     * */
     auto getDuration() const -> unsigned long;

    bool isSingleShot() const noexcept;

    friend class Scheduler;

protected:
    /**
     * This method is call when the scheduler decide
     * */
    auto trigger() -> void;
};
