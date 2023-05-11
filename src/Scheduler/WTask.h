#pragma once

#include "Scheduler/WObject.h"

class WTask:    public WObject,
                public QRunnable,

{
public:
    explicit WTask(WObject *parent = nullptr);
    ~WTask() override = default;

    void run() override;

    Q_DISABLE_COPY(WTask);

signals:
    /**
     * @ensures task == this
     * */
    void finished(WTask *task);
};

