#pragma once


#include <QString>
#include <QStandardPaths>

namespace get_path_application
{
    /**
     * this function return a writable position for writernote
    */
    inline QString exe()
    {
        return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    }
};
