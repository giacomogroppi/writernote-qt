#include "WTask.h"

WTask::WTask(QObject *parent)
    : QObject(parent)
{
}

void WTask::run()
{
    emit this->finished(this);
}
