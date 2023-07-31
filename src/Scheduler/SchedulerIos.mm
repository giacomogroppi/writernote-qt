#include "Scheduler.h"
#import <Foundation/Foundation.h>
#include "platform.h"

#if is_ios
static auto executeOnMainThread (dispatch_block_t method)
{
    if (![NSThread isMainThread])
        dispatch_sync(dispatch_get_main_queue(), method);
    else
        method();
}

void Scheduler::addTaskMainThread(WTask *task)
{
    W_ASSERT(task);
    executeOnMainThread(^{
        Scheduler::manageExecution(task);
    });
}
#endif // is_ios
