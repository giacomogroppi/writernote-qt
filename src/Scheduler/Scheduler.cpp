#include "Scheduler.h"
#include "core/WMutexLocker.h"
#include "utils/threadcount.h"

Scheduler::Scheduler(QObject *parent)
    : QObject(parent), _need_to_sort(false)
{
    for (int i = 0; i < threadCount::count(); i++) {
        this->_threads.push_back(new QThread(nullptr));
    }
}

Scheduler::~Scheduler()
{
    for (auto &thread : _threads) {
        if (thread->isRunning())
            thread->quit();
        delete thread;
    }
}

void Scheduler::createHeap()
{
    std::make_heap(_pools.begin(), _pools.end(), [](WPool *a, WPool *b) {
        return a->getPriority() < b->getPriority();
    });
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

void Scheduler::threadFinished()
{
    WMutexLocker _(_pool_locker);

    if (this->_need_to_sort.value()) {

    }

    bool last;
    int i;
    for (i = 0; i < _threads.size(); i++) {

    }
}

bool Scheduler::is_heap() const
{
    WMutexLocker _(this->_pool_locker);
    const auto r = this->_pools.at(0)->getPriority();
    for (int i = 1; i < this->_pools.size(); i++) {
        if (r < this->_pools.at(i)->getPriority())
            return false;
    }
    return true;
}
