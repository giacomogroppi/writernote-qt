#pragma once

#include <functional>
#include <QtGlobal>
#include <QObject>
#include <queue>
#include "WPool.h"
#include "core/WMutex.h"
#include "core/AtomicSafe.h"
#include <QThread>

class SchedulerThreadData {
public:
    bool *thread;
};

class Scheduler final: public QObject{
private:
    std::vector<WPool *> _pools;
    mutable WMutex _pool_locker;
    std::vector<QThread *> _threads;

    /**
     * //@ requires
     * //@   _pools is locked
     * //@ ensures
     * //@  (\forall int i; 0 <= i < _pools.size();
     * //@      _pools[0]->getPriority() >= _pools[i]->getPriority())
     * */
    void createHeap();

    /**
     * //@ requires
     * //@   _pool_locker is not locked
     * //@ ensures
     * //@   (*_pool_locker is not locked*) &&
     * //@   \result <==> (\forall int i; 0 <= i < _pools.size();
     * //@       _pools.at(0)->getPriority() >= _pools.at(i)->getPriority())
     * */
    bool is_heap() const;

    AtomicSafe<bool> _need_to_sort;

public:
    explicit Scheduler(QObject *parent = nullptr);
    ~Scheduler() final;

    void removePool(WPool *task);
    void addPool(WPool *task);

    Q_DISABLE_COPY(Scheduler);
private slots:
    void onPriorityChanged();
    void threadFinished();
};

