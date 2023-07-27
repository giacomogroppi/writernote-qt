#include "WTask.h"
#include "Scheduler/WObject.h"
#include "core/WMutexLocker.h"

WTask::WTask(WObject *parent, bool deleteLater)
    : WObject(parent)
    , _waiterLock()
    , _waiter(0)
    , _deleteLater(deleteLater)
    , _hasFinish(false)
{
}

void WTask::releaseJoiner() noexcept
{
    WMutexLocker _(this->_waiterLock);
    this->_sem.release(_waiter);
    _waiter = 0;
    _hasFinish = true;
}

void WTask::join()
{
    {
        WMutexLocker _(this->_waiterLock);

        if (_hasFinish)
            return;

        _waiter ++;
    }
    this->_sem.acquire();
}
