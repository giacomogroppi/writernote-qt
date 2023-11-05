#include "Scheduler.h"

#ifdef USE_QT

#include <QTimer>
#include <QThread>
#include <QThreadPool>
#include <QRunnable>
#include <QAbstractEventDispatcher>
#include "SchedulerQt.h"
#include <QGuiApplication>

SchedulerQt::SchedulerQt(QObject *parent)
    : QObject(parent)
{
    auto *i = QGuiApplication::instance();
    i->installEventFilter(this);
}

auto SchedulerQt::eventFilter(QObject *, QEvent *event) -> bool
{
    W_ASSERT(this->thread()->currentThreadId() == QGuiApplication::instance()->thread()->currentThreadId());

    if (auto *e = dynamic_cast<SchedulerEvent*>(event)) {
        e->run();
        return true;
    }
    return false;
}

static SchedulerQt *instance;

auto Scheduler::initMainThread() -> void
{
    instance = new SchedulerQt(nullptr);
}

auto Scheduler::endMainThread() -> void
{
    delete instance;
}

auto Scheduler::addTaskMainThread(Ptr<WTask> task) -> void
{
    task->beforeRun();

    if constexpr (WUtils::debug_enable()) {
        if (task->isDeleteLater())
            W_ASSERT(task.numberOfRef() == 1);
    }

    QGuiApplication::postEvent(instance, new SchedulerEvent(std::move(task)));
}

auto Scheduler::numberOfThread() -> Unsigned
{
    const auto n = QThread::idealThreadCount();
    return Unsigned(static_cast<unsigned>(n));
}

#endif // USE_QT
