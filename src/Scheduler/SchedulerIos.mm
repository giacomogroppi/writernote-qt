#include "Scheduler.h"
#import <Foundation/Foundation.h>
#include "platform.h"

#if is_ios
static auto executeOnMainThread (dispatch_block_t method)
{
    dispatch_async(dispatch_get_main_queue(), method);
}

void Scheduler::addTaskMainThread(WTask *task)
{
    W_ASSERT(task);
    executeOnMainThread(^{
        Scheduler::manageExecution(task);
    });
}
#endif // is_ios
