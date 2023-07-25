#pragma once

#ifdef USE_QT
# include "WImageQt.h"
#else
    // TODO: manage android import
# import "WImageIos.h"
#endif // USE_QT
