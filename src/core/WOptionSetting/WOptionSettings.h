#pragma once

#include "utils/platform.h"

#if defined(USE_QT)
# include "WOptionSettingsQt.h"
#else
# include "WOptionSettingsMobile.h"
#endif
