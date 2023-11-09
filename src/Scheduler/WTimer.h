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
    std::atomic_bool _isActive;
    bool _isSingleShot;
    bool _executionMainThread;
    unsigned long _millisecond;
    unsigned long _timeStart;

    Fn<void()> _function;
public:
    struct Flag {
        enum flag {
            Zero = 0x0,
            onMainThread = 0x1,
            singleShot = 0x2
        };
    };

    WTimer(WObject *parent, Fn<void()> function, int millisecond, WFlags<Flag::flag> flags);
    ~WTimer() override;

    bool isActive() const;
    void stop();
    void start(int millisecond = -1);
    void setSingleShot(bool singleShot);

    static auto now() -> unsigned long;

    auto isExecutionMainThread() const -> bool;

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

    auto setActive(bool active) -> void;
};

inline auto WTimer::setActive(bool active) -> void
{
    this->_isActive = active;
}

inline auto WTimer::isExecutionMainThread() const -> bool
{
    return this->_executionMainThread;
}

#ifdef USE_QT
template <class T>
inline Q_CORE_EXPORT QDebug operator<<(QDebug d, const WTimer *p)
{
    d.nospace() << "WTimer(";

    d.nospace() << p->getEnd();

    d.nospace() << ")";

    return d.space();
}
#endif // USE_QT

inline auto WTimer::getDuration() const -> unsigned long
{
    WMutexLocker _(this->_lock);
    return this->_millisecond;
}
