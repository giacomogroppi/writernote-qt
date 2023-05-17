#pragma once


#include "core/WString.h"
#include <QStandardPaths>

namespace get_path_application
{
    /**
     * this function return a writable position for writernote
    */
    inline WString exe()
    {
        return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    }
};
