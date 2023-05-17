#include "WPool.h"
#include "utils/WCommonScript.h"

/*
WPool::WPool(WObject *parent)
    : WObject(parent)
    , _priority(0)
    , _active_thread(0)
{

}

void WPool::setPriority(int priority)
{
    W_ASSERT(priority >= 0);
    this->_priority = priority;
    W_EMIT_0(priorityChanged);
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

void WPool::addTask(WTask *task)
{
    W_ASSERT(this->_active_thread == 0);
    WMutexLocker _(this->_tasks_locker);
    W_ASSERT(WCommonScript::contains(_tasks, task) == 0);
    this->_tasks.push_back(task);

    WObject::connect(task, &WTask::finished,
                     this, &WPool::threadFinish);
}

void WPool::threadFinish(WTask *)
{
    --_active_thread;
}
*/
