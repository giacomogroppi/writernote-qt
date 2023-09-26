#pragma once

#include "utils/platform.h"

class WOptionSettingsDefault {
public:
    /**
     * This is the name used for WOptionSettings for identify the
     * position in which the application should save.
    */
    static constexpr auto namePathSaving = "path_saving";
};

#if defined(USE_QT)
# include "WOptionSettingsQt.h"
#else
# include "WOptionSettingsMobile.h"
#endif
