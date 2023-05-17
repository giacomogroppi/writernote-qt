#include "Scheduler.h"
#include "core/WMutexLocker.h"
#include "utils/threadcount.h"

/*
Scheduler::Scheduler(WObject *parent)
    : WObject(parent)
    , _need_to_sort(false)
{
}

Scheduler::~Scheduler()
{
}

void Scheduler::createHeap()
{
    std::make_heap(_pools_active.begin(),
                   _pools_active.end(),
                   [](WPool *a, WPool *b) {
        return a->getPriority() < b->getPriority();
    });
    W_ASSERT(this->is_heap());
}

void Scheduler::onPriorityChanged()
{
    this->_need_to_sort = true;
}

void Scheduler::addPool(WPool *task)
{
    WMutexLocker _(_pool_active_locker);
    _pools_active.push_back(task);

    this->createHeap();

    WObject::connect(task, &WPool::priorityChanged,
                     this, &Scheduler::onPriorityChanged);
    WObject::connect(task, &WPool::jobsFinished,
                     this, &Scheduler::onPoolEnd);
    WObject::connect(task, &WPool::jobsAvaliable,
                     this, &Scheduler::onJobAvailable);
}

void Scheduler::onJobAvailable(WPool *pool)
{
    _pool_active_locker.lock();
    _pools_active.push_back(pool);
    _pool_active_locker.unlock();

    _pool_not_active_locker.lock();
    WCommonScript::removeFromArray(_pools_not_active, pool);
    _pool_not_active_locker.lock();
}

bool Scheduler::is_heap() const
{
    WMutexLocker _(this->_pool_active_locker);
    const auto r = this->_pools_active.at(0)->getPriority();

    for (int i = 1; i < (int)this->_pools_active.size(); i++) {
        if (r < this->_pools_active.at(i)->getPriority())
            return false;
    }
    return true;
}

void Scheduler::onPoolEnd(WPool *pool)
{
    W_ASSERT(this->_pool_active_locker.isLocked());
    W_ASSERT(std::count(_pools_active.begin(),
                        _pools_active.end(),
                        pool) == 1);
    W_ASSERT(std::count(_pools_not_active.begin(),
                        _pools_not_active.end(),
                        pool) == 0);

    if (this->_need_to_sort.value()) {
        this->createHeap();
        this->_need_to_sort = false;
    }

    _pool_active_locker.lock();
    WCommonScript::removeFromArray(_pools_active, pool);
    _pool_active_locker.unlock();

    _pool_not_active_locker.lock();
    _pools_not_active.push_back(pool);
    _pool_not_active_locker.unlock();

    WObject::disconnect(pool, &WPool::priorityChanged,
                     this, &Scheduler::onPriorityChanged);
    WObject::disconnect(pool, &WPool::jobsFinished,
                     this, &Scheduler::onPoolEnd);

    this->startNewPool();
}

void Scheduler::startNewPool()
{
    this->_pools_active.at(0)->startJobs(getThreadPool());
}

QThreadPool *Scheduler::getThreadPool()
{
    return &this->_threads;
}
*/
