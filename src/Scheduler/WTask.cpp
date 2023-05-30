#include "WTask.h"
#include "Scheduler/WObject.h"

WTask::WTask(WObject *parent)
    : WObject(parent)
    , _waiter(0)
{
}

void WTask::run()
{
    W_EMIT_1(finished, this);
}

void WTask::releaseJoiner() noexcept
{
    for (int i = 0; i < this->_waiter; i++) {
        this->_sem.release();
    }
}

void WTask::join()
{
    _waiter ++;
    this->_sem.acquire();
}
