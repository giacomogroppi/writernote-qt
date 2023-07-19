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
    , _currentId(0)
    , _isActive(false)
    , _millisecond(millisecond)
    , _function(std::move(function))
{

}

void WTimer::start(int millisecond)
{
    if (millisecond != -1)
        this->_millisecond = millisecond;

    this->_lock.lock();

    const auto myId = _currentId;
    this->_currentId ++;

    this->_lock.unlock();

    std::thread([this, myId]() -> void {
        for (;;) {
            _isActive = true;
            std::this_thread::sleep_for(std::chrono::milliseconds (this->_millisecond));

            {
                WMutexLocker _(this->_lock);
                if (myId < _currentId) {
                    // we need to kill this thread since someone call exit
                    return;
                }
            }

            Scheduler::addTaskMainThread(
                    new TaskTimer(this->_function)
            );

            if (isSingleShot())
                return;
        }
    }).detach();
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

void WTimer::stop()
{
    WMutexLocker _(this->_lock);
    this->_currentId ++;
}

