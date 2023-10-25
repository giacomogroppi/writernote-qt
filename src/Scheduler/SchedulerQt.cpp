#include "Scheduler.h"

#ifdef USE_QT

#include <QTimer>
#include <QThread>
#include <QThreadPool>

auto Scheduler::addTaskMainThread(Ptr<WTask> task) -> void
{
    // TODO: use qt scheduler
    W_ASSERT(task);

#ifdef DEBUGINFO
    if (task->isDeleteLater())
        W_ASSERT(task.numberOfRef() == 1);
#endif // DEBUGINFO

    WDebug(true, "Add task to main thread");

    auto method = [task = std::move(task)] () mutable {
        WDebug(true, "Execute");
        manageExecution(std::move(task));
    };

    QTimer::singleShot(0, std::move(method));

    //method();

#ifdef DEBUGINFO
    W_ASSERT(task == Ptr<WTask>());
#endif // DEBUGINFO
}

auto Scheduler::numberOfThread() -> Unsigned
{
    const auto n = QThread::idealThreadCount();
    return Unsigned(static_cast<unsigned>(n));
}

#endif // USE_QT
