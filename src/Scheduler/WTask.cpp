#include "WTask.h"
#include "Scheduler/WObject.h"
#include "core/WMutexLocker.h"
#include "core/WVector.h"
#include "Scheduler.h"

WTask::WTask(WObject *parent, WFlags<Flag> deleteLater)
    : WObject(parent)
    , _waiterLock()
    , _deleteLater((deleteLater & Flag::DeleteLater) != 0x0)
    , _hasFinish(false)
{

}

void WTask::releaseJoiner() noexcept
{
    WMutexLocker _(this->_waiterLock);

    _conditionalVariable.notify_all();
    _hasFinish = true;
}

WTask::~WTask() = default;

void WTask::beforeRun()
{
    this->_hasFinish = false;
}

void WTask::join()
{
    W_ASSERT(not isDeleteLater());

    if (_hasFinish)
        return;

    Scheduler::joinThread(_hasFinish);

    std::unique_lock guard (_waiterLock);

    while (not _hasFinish)
        _conditionalVariable.wait(guard);
}
