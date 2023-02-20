#include "WPool.h"
#include "utils/WCommonScript.h"

WPool::WPool(QObject *parent)
    : QObject(parent)
    , _priority(0)
{

}

void WPool::setPriority(int priority)
{
    W_ASSERT(priority >= 0);
    this->_priority = priority;
}
