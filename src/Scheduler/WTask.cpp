#include "WTask.h"

WTask::WTask(WObject *parent)
    : WObject(parent)
{
}

void WTask::run()
{
    emit this->finished(this);
}
