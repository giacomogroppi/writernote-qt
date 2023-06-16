#pragma once

#include "Scheduler/WObject.h"
#include "core/WSemaphore.h"
#include "utils/WCommonScript.h"

class WTask: public WObject
{
private:
    int _waiter;
    WSemaphore _sem;
public:
    explicit WTask(WObject *parent = nullptr);
    ~WTask() override = default;

    virtual void run() = 0;
    void join();

    void releaseJoiner() noexcept;

    WDISABILE_COPY(WTask);

    /**
     * @ensures task == this
     * */
    W_EMITTABLE_1(finished, WTask*, task)
};

