#pragma once

#include "utils/platform.h"

#ifdef USE_QT
# include "WByteArrayQtPrivate.h"
#elif is_ios || is_mac
# include "WByteArrayUniversalPrivate.h"
#endif // USE_QT
