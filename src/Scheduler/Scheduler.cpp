#include "Scheduler.h"
#include "core/WMutexLocker.h"
#include "utils/threadcount.h"
#include <QPointer>

Scheduler::Scheduler(QObject *parent)
    : QObject(parent)
    , _need_to_sort(false)
{
}

Scheduler::~Scheduler()
{
}

void Scheduler::createHeap()
{
    std::make_heap(_pools.begin(), _pools.end(), [](WPool *a, WPool *b) {
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
    WMutexLocker _(_pool_locker);
    _pools.push_back(task);

    this->createHeap();

    QObject::connect(task, &WPool::priorityChanged,
                     this, &Scheduler::onPriorityChanged);
}

bool Scheduler::is_heap() const
{
    WMutexLocker _(this->_pool_locker);
    const auto r = this->_pools.at(0)->getPriority();

    for (int i = 1; i < (int)this->_pools.size(); i++) {
        if (r < this->_pools.at(i)->getPriority())
            return false;
    }
    return true;
}

void Scheduler::allThreadFinished()
{
    W_ASSERT(this->_pool_locker.isLocked());

    if (this->_need_to_sort.value()) {
        this->createHeap();
        this->_need_to_sort = false;
    }

    this->startNewPool();
}

void Scheduler::startNewPool()
{
    this->_pools.at(0)->startJobs(getThreadPool());
}

QThreadPool *Scheduler::getThreadPool()
{
    return &this->_threads;
}
