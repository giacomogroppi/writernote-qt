#include "Scheduler.h"
#include "core/WMutexLocker.h"
#include "utils/threadcount.h"


Scheduler::Scheduler(WObject *parent)
    : WObject(parent)
    , _need_to_sort(false)
    , _threads()
{
    // TODO --> change this parameter
    _threads.reserve(8);
    std::vector<std::thread> thread;

    for (int i = 0; i < 8; i++) {
        _threads.append(std::thread([this, i]() {
            const auto isMainThread = i == 0;

            WSemaphore *sem = isMainThread ? &this->_semMain : &this->_sem;
            WMutex *mux = isMainThread ? &this->_lockMain : &this->_lock_heap;

            // loop
            for (;;) {
                WTask *task;
                sem->acquire();

                if (this->needToDie())
                    return;

                mux->lock();
                task = this->_pools_main_thread.takeFirst();
                mux->unlock();

                task->run();
                task->releaseJoiner();
            }
        }));
    }
}

Scheduler::~Scheduler()
{
}

void Scheduler::createHeap()
{
    /*
    std::make_heap(_pools_not_active.begin(),
                   _pools_not_active.end(),
                   [](WTask *a, WTask *b) {
        return a->getPriority() < b->getPriority();
    });
     */
    W_ASSERT(this->is_heap());
}

void Scheduler::addTask(WTask *task)
{
    WMutexLocker _(_lock_heap);

    this->_need_to_sort = true;
}

bool Scheduler::is_heap() const
{
    return true;
    /*
    WMutexLocker _(this->_pool_active_locker);
    const auto r = this->_pools_not_active.at(0)->getPriority();

    for (int i = 1; i < (int)this->_pools_not_active.size(); i++) {
        if (r < this->_pools_not_active.at(i)->getPriority())
            return false;
    }
    return true;
*/
}

void Scheduler::loop(int index)
{
    for (;;) {
        this->_sem.acquire();
        this->_lock_heap.lock();
        auto *task = this->_pools_not_active.takeAt(0);
        this->_lock_heap.unlock();
        task->run();
    }
}

constexpr bool Scheduler::needToDie() const noexcept
{
    return this->_needToDie;
}

