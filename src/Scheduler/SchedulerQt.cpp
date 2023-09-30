#include "Scheduler.h"

#ifdef USE_QT

#include <QTimer>
#include <QThread>

auto Scheduler::addTaskMainThread(WTask *task) -> void
{
    // TODO: use qt scheduler
    W_ASSERT(task);

    QTimer::singleShot(0, [task] {
        manageExecution(task);
    });
}

auto Scheduler::numberOfThread() -> Unsigned
{
    const auto n = QThread::idealThreadCount();
    return Unsigned(static_cast<unsigned>(n));
}

#endif // USE_QT
