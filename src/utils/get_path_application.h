#ifndef GET_PATH_APPLICATION_H
#define GET_PATH_APPLICATION_H

#include <QString>
#include <QStandardPaths>

class get_path_application
{
public:
    /*
     * this function return a writable position for writernote
    */
    static QString exe();
};

inline QString get_path_application::exe()
{
     return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}

#endif // GET_PATH_APPLICATION_H
