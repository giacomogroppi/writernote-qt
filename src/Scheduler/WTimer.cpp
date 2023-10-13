#include "WTimer.h"
#include "Scheduler.h"
#include <chrono>
#include <utility>

WTimer::WTimer(WObject *parent, Fn<void()> function, int millisecond, bool onMainThread)
    : WObject(parent)
    , _isActive(false)
    , _isSingleShot(false)
    , _executionMainThread(onMainThread)
    , _millisecond(millisecond)
    , _timeStart(0)
    , _function(std::move(function))
{
}

WTimer::~WTimer()
{
    WDebug(false, static_cast<void*>(this) << "Call destructor");
}


void WTimer::start(int millisecond)
{
    WDebug(false, static_cast<void*>(this) << "Call start");

    using namespace std::chrono;

    if (millisecond != -1)
        this->_millisecond = millisecond;

    this->_isActive = true;
    this->_timeStart = now();

    Scheduler::getInstance().addTimer(this);
}

bool WTimer::isActive() const
{
    WMutexLocker _(this->_lock);
    return this->_isActive;
}

bool WTimer::isSingleShot() const noexcept
{
    WMutexLocker _(this->_lock);
    return this->_isSingleShot;
}

void WTimer::setSingleShot(bool singleShot)
{
    WMutexLocker _(this->_lock);
    this->_isSingleShot = singleShot;
}

void WTimer::stop()
{
    W_ASSERT(this->isActive());
    Scheduler::getInstance().removeTimer(this);
}

/**
 * TODO: move into .h file
 */
auto WTimer::getDuration() const -> unsigned long
{
    WMutexLocker _(this->_lock);
    return this->_millisecond;
}

auto WTimer::trigger() -> void
{
    WDebug(false, static_cast<void*>(this) << "Call trigger");

    WMutexLocker _(_lock);
    this->_function();
}

auto WTimer::getEnd() const -> unsigned long
{
    WMutexLocker _(_lock);
    return this->_millisecond + this->_timeStart;
}

auto WTimer::now() -> unsigned long
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

