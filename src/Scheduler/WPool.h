#pragma once

#include <QThreadPool>
#include "WTask.h"
#include "core/AtomicSafe.h"

class WPool: public QObject {
    Q_OBJECT
private:
    int _priority;
    std::vector<WTask *> _tasks;
    AtomicSafe<int> _active_thread;
public:
    explicit WPool(QObject *parent = nullptr);
    ~WPool() override = default;

    void addTask(WTask *task);
    void removeTask(WTask *task);

    /**
     * @ensures: \result >= 0
     */
    [[nodiscard]] virtual int getPriority() const = 0;

    [[nodiscard]] virtual bool isJobsAvaliable() const = 0;

    void startJobs(QThreadPool *pool);

protected:
    void setPriority(int priority);

    Q_DISABLE_COPY(WPool);

signals:
    void priorityChanged();
    /**
     * @ensures pool == this
     * */
    void jobsAvaliable(WPool *pool);

    /**
     * @ensures pool == this
     * */
    void jobsFinished(WPool *pool);
};


