#include "Scheduler.h"
#include "core/WMutexLocker.h"
#include "utils/threadcount.h"

static Scheduler *instance;

Scheduler::Scheduler()
    : WObject(nullptr)
    , _need_to_sort(false)
    , _threads()
    , _needToDie(false)
    , _semGeneral(0)
    , _semMain(0)
{
    W_ASSERT(instance == nullptr);

    instance = this;
    // TODO --> change this parameter
    _threads.reserve(8);
    std::vector<std::thread> thread;

    for (int i = 0; i < 8; i++) {
        _threads.append(std::thread([this, i]() {
            const auto isMainThread = i == 0;

            WSemaphore *sem =           isMainThread ? &this->_semMain      : &this->_semGeneral;
            WMutex *mux =               isMainThread ? &this->_lockMain     : &this->_lockGeneric;
            WList<WTask *> *tasksHeap = isMainThread ? &this->_task_Main    : &this->_task_General;

            // loop
            for (;;) {
                WTask *task;
                sem->acquire();

                if (this->needToDie()) {
                    return;
                }

                {
                    WMutexLocker _(*mux);
                    task = tasksHeap->takeFirst();
                }

                const auto needToDeleteLater = task->isDeleteLater();

                task->run();
                task->releaseJoiner();

                if (needToDeleteLater) {
                    delete task;
                }
            }
        }));
    }
}

Scheduler::~Scheduler()
{
    _needToDieLock.lock();
    this->_needToDie = true;

    this->_semMain.release(1);
    this->_semGeneral.release(_threads.size());

    _needToDieLock.unlock();

    for (auto &ref: _threads)
        ref.join();

    for (auto *t: this->_task_General)
        t->releaseJoiner();

    for (auto *t: this->_task_Main)
        t->releaseJoiner();

    instance = nullptr;
}

void Scheduler::createHeap()
{
    /*
    std::make_heap(_task_General.begin(),
                   _task_General.end(),
                   [](WTask *a, WTask *b) {
        return a->getPriority() < b->getPriority();
    });
     */
    W_ASSERT(this->is_heap());
}

bool Scheduler::is_heap() const
{
    return true;
    /*
    WMutexLocker _(this->_pool_active_locker);
    const auto r = this->_task_General.at(0)->getPriority();

    for (int i = 1; i < (int)this->_task_General.size(); i++) {
        if (r < this->_task_General.at(i)->getPriority())
            return false;
    }
    return true;
*/
}

bool Scheduler::needToDie() const noexcept
{
    WMutexLocker locker (this->_needToDieLock);
    return this->_needToDie;
}

void Scheduler::addTaskMainThread(WTask *task)
{
    W_ASSERT(task);

    WMutexLocker _(instance->_lockMain);
    instance->_task_Main.append(task);

    instance->_semMain.release();
}

Scheduler &Scheduler::getInstance()
{
    return *instance;
}