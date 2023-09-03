#include "Scheduler.h"

#ifdef USE_QT
void Scheduler::addTaskMainThread(WTask *task)
{
    // TODO: use qt scheduler
    W_ASSERT(task);
    auto &instance = Scheduler::getInstance();
    
    WMutexLocker _(instance._lockMain);
    instance._task_Main.append(task);

    instance._semMain.release();
}
#endif // USE_QT
