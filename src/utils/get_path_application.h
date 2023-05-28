#pragma once

#ifdef USE_QT
# include "QtCore/qstandardpaths.h"
#endif
#include "core/WString.h"

namespace get_path_application
{
    /**
     * this function return a writable position for writernote
    */
    inline WString exe()
    {
    #ifdef USE_QT
        return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    #else

    #endif // USE_QT
    }
};
