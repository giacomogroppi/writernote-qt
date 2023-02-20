#pragma once

#include "WTask.h"

class WPool: public QObject {
    Q_OBJECT
private:
    int _priority;
    std::vector<WTask *> _tasks;
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

protected:
    void setPriority(int priority);

    Q_DISABLE_COPY(WPool);

signals:
    void priorityChanged();
    void jobsAvaliable();
};


