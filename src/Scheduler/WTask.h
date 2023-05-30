#pragma once

#include "Scheduler/WObject.h"
#include "core/WSemaphore.h"

class WTask: public WObject
{
private:
    int _waiter;
    WSemaphore _sem;
public:
    explicit WTask(WObject *parent = nullptr);
    ~WTask() override = default;

    void run();
    void join();

    void releaseJoiner() noexcept;

    /**
     * @ensures task == this
     * */
    W_EMITTABLE_1(finished, WTask*, task)
};

