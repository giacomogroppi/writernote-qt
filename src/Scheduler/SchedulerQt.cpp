#include "Scheduler.h"

#ifdef USE_QT

#include <QTimer>
#include <QThread>

auto Scheduler::addTaskMainThread(Ptr<WTask> task) -> void
{
    // TODO: use qt scheduler
    W_ASSERT(task);

    QTimer::singleShot(0, [task = std::move(task)] () mutable {
        manageExecution(std::move(task));
    });
}

auto Scheduler::numberOfThread() -> Unsigned
{
    const auto n = QThread::idealThreadCount();
    return Unsigned(static_cast<unsigned>(n));
}

#endif // USE_QT
