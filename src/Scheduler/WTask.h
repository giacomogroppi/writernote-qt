#pragma once

#include "Scheduler/WObject.h"

class WTask:    public WObject
{
public:
    explicit WTask(WObject *parent = nullptr);
    ~WTask() override = default;

    void run();

    /**
     * @ensures task == this
     * */
    W_EMITTABLE_1(finished, WTask*, task);
};

