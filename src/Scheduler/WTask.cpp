#include "WTask.h"
#include "Scheduler/WObject.h"
#include "core/WMutexLocker.h"
#include "core/WVector.h"
#include "Scheduler.h"

WTask::WTask(WObject *parent, bool deleteLater)
    : WObject(parent)
    , _waiterLock()
    , _deleteLater(deleteLater)
    , _hasFinish(false)
    , _threadsCreated(0)
{
}

void WTask::releaseJoiner() noexcept
{
    WMutexLocker _(this->_waiterLock);

    _conditionalVariable.notify_all();
    _hasFinish = true;
}

WTask::~WTask() = default;

void WTask::join()
{
    W_ASSERT(not isDeleteLater());

    Scheduler::joinThread(_hasFinish);

    std::unique_lock guard (_waiterLock);

    while (_hasFinish)
        _conditionalVariable.wait(guard);
}
