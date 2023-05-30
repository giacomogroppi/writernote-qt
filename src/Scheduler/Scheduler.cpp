#include "Scheduler.h"
#include "core/WMutexLocker.h"
#include "utils/threadcount.h"

static Scheduler *instance;

Scheduler::Scheduler(WObject *parent)
    : WObject(parent)
    , _need_to_sort(false)
    , _threads()
{
    instance = this;
    // TODO --> change this parameter
    _threads.reserve(8);
    std::vector<std::thread> thread;

    for (int i = 0; i < 8; i++) {
        _threads.append(std::thread([this, i]() {
            const auto isMainThread = i == 0;

            WSemaphore *sem = isMainThread ? &this->_semMain : &this->_semGeneral;
            WMutex *mux = isMainThread ? &this->_lockMain : &this->_lock_heap;
            WVector<WTask *> *tasksHeap = isMainThread ? &this->_Task_Main : &_Task_General;

            // loop
            for (;;) {
                WTask *task;
                sem->acquire();

                if (this->needToDie())
                    return;

                mux->lock();
                task = this->_Task_Main.takeFirst();
                mux->unlock();

                task->run();
                task->releaseJoiner();
            }
        }));
    }
}

Scheduler::~Scheduler() = default;

void Scheduler::createHeap()
{
    /*
    std::make_heap(_Task_General.begin(),
                   _Task_General.end(),
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
    const auto r = this->_Task_General.at(0)->getPriority();

    for (int i = 1; i < (int)this->_Task_General.size(); i++) {
        if (r < this->_Task_General.at(i)->getPriority())
            return false;
    }
    return true;
*/
}

constexpr bool Scheduler::needToDie() const noexcept
{
    return this->_needToDie;
}

void Scheduler::addTaskMainThread(WTask *task)
{
    W_ASSERT(task);

    WMutexLocker _(instance->_lockMain);
    instance->_Task_Main.append(task);
    instance->_semMain.release();
}

