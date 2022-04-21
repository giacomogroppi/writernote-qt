#ifndef CORE_H
#define CORE_H

#include "utils/common_script.h"
#include "utils/platform.h"

extern bool __is_mobile_view;

namespace core{

force_inline bool is_mobile_view()
{
#if is_android || is_ios
    return true;
#else
    return __is_mobile_view;
#endif
}

force_inline void set_mobile_view(cbool is_tablet)
{
    if(!is_mobile_static())
        __is_mobile_view = is_tablet;
}

}


#endif // CORE_H
