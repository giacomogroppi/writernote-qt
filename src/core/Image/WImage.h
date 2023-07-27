#pragma once

#include "utils/platform.h"

#ifdef USE_QT
# include "WImageQt.h"
#elif is_ios == 1 || is_mac == 1
    // TODO: manage android import
# include "WImageIos.h"
#endif // USE_QT
