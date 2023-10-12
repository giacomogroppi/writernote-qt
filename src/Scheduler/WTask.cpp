#include "WTask.h"
#include "Scheduler/WObject.h"
#include "core/WMutexLocker.h"
#include "core/WVector.h"
#include "Scheduler.h"

std::atomic<unsigned long> identifier;

WTask::WTask(WObject *parent, bool deleteLater)
    : WObject(parent)
    , _waiterLock()
    , _waiter(0)
    , _deleteLater(deleteLater)
    , _hasFinish(false)
    , _identifier(identifier++)
    , _threadsCreated(0)
{
    WDebug(false, "Create object" << static_cast<void*>(this));
}

void WTask::releaseJoiner() noexcept
{
    WMutexLocker _(this->_waiterLock);
    this->_sem.release(_waiter);
    _waiter = 0;
    _hasFinish = true;
}

WTask::~WTask() = default;

void WTask::join()
{
    W_ASSERT(not isDeleteLater());
    {
        WMutexLocker _(this->_waiterLock);

        if (_hasFinish)
            return;

        _waiter ++;
    }

    Scheduler::joinThread(this->_threadsCreated, identifier);

    this->_sem.acquire();
}
