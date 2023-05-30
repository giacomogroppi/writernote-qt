//
// Created by Giacomo Groppi on 10/05/23.
//

#include "WTimer.h"
#include "Scheduler.h"

#include <utility>

class TaskTimer : public WTask {
    std::function<void()> _function;
public:
    explicit TaskTimer (const std::function<void()> &function) : _function(function) {}
    void run() final
    {
        _function();
    };
};

WTimer::WTimer(WObject *parent, std::function<void()> function, int millisecond)
    : WObject(parent)
    , _millisecond(millisecond)
    , _function(std::move(function))
    , _isActive(false)
{

}

void WTimer::start(int millisecond)
{
    if (millisecond != -1)
        this->_millisecond = millisecond;

    this->_thread = std::thread([this]() -> void {
        for (;;) {
            _isActive = true;
            std::this_thread::sleep_for(std::chrono::milliseconds (this->_millisecond));
            Scheduler::addTaskMainThread(
                    new TaskTimer(this->_function)
            );

            if (isSingleShot())
                return;
        }
    });
}

bool WTimer::isActive() const
{
    WMutexLocker _(this->_lock);
    return this->_isActive;
}

bool WTimer::isSingleShot() const noexcept
{
    WMutexLocker _(this->_lock);
    return this->_isActive;
}

void WTimer::setSingleShot(bool singleShot)
{
    WMutexLocker _(this->_lock);
    this->_isActive = singleShot;
}
