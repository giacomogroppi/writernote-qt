#include "WTask.h"
#include "Scheduler/WObject.h"
#include "core/WMutexLocker.h"
#include "core/WVector.h"

static WMutex mutex;
static WVector<WTask*> items;

WTask::WTask(WObject *parent, bool deleteLater)
    : WObject(parent)
    , _waiterLock()
    , _waiter(0)
    , _deleteLater(deleteLater)
    , _hasFinish(false)
{
    WDebug(false, "Create object" << static_cast<void*>(this));
    WMutexLocker _(mutex);
    items.append(this);
}

void WTask::releaseJoiner() noexcept
{
    WMutexLocker _(this->_waiterLock);
    this->_sem.release(_waiter);
    _waiter = 0;
    _hasFinish = true;
}

WTask::~WTask()
{
    WDebug(false, "Delete object" << static_cast<void*>(this));

    WMutexLocker __(mutex);
    W_ASSERT(items.contains(this));
    items.removeObjects(this);
}

void WTask::join()
{
    W_ASSERT(not isDeleteLater());
    {
        WMutexLocker _(this->_waiterLock);

        if (_hasFinish)
            return;

        _waiter ++;
    }
    this->_sem.acquire();
}
