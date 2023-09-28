#pragma once

#include "utils/platform.h"

#ifdef USE_QT
# include "AudioRecorderQt.h"
#endif // USE_QT

#if is_ios
# include "AudioRecorderIOS.h"
#endif // is_ios
