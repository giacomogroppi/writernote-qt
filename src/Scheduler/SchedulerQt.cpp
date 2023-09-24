#include "Scheduler.h"

#ifdef USE_QT
#include <QTimer>
auto Scheduler::addTaskMainThread(WTask *task) -> void
{
    // TODO: use qt scheduler
    W_ASSERT(task);

    QTimer::singleShot(0, [task] {
        manageExecution(task);
    });
}
#endif // USE_QT
