#include "WTask.h"
#include "Scheduler/WObject.h"

WTask::WTask(WObject *parent)
    : WObject(parent)
{
}

void WTask::run()
{
    W_EMIT_1(finished, this);
}
