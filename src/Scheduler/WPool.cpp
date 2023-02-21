#include "WPool.h"
#include "utils/WCommonScript.h"

WPool::WPool(QObject *parent)
    : QObject(parent)
    , _priority(0)
    , _active_thread(0)
{

}

void WPool::setPriority(int priority)
{
    W_ASSERT(priority >= 0);
    this->_priority = priority;
    emit this->priorityChanged();
}

void WPool::startJobs(QThreadPool *pool)
{
    W_ASSERT(pool != nullptr);
    W_ASSERT(this->_active_thread.value() == 0);
    for (auto &task : this->_tasks) {
        pool->start(task);
    }

    _active_thread = static_cast<int>(this->_tasks.size());
}
