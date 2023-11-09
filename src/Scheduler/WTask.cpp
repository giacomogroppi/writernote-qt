#include "WTask.h"
#include "Scheduler/WObject.h"
#include "core/WMutexLocker.h"
#include "Scheduler.h"
#include "WTaskAllocator.h"

WTask::WTask(WObject *parent, WFlags<Flag> deleteLater)
    : WObject(parent)
    , _d(WTaskAllocator::get())
    , _deleteLater((deleteLater & Flag::DeleteLater) != 0x0)
    , _hasFinish(false)
{

}

void WTask::releaseJoiner() noexcept
{
    WMutexLocker _(this->_d->_waiterLock);

    _d->_conditionalVariable.notify_all();
    _hasFinish = true;
}

WTask::~WTask()
{
    WTaskAllocator::put(_d);
    _d = nullptr;
}

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

    std::unique_lock guard (_d->_waiterLock);

    while (not _hasFinish)
        _d->_conditionalVariable.wait(guard);
}
