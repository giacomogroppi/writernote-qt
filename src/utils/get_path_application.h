#ifndef GET_PATH_APPLICATION_H
#define GET_PATH_APPLICATION_H

#include <QString>

#ifdef ANDROID
class get_path_application
{
public:
    static QString exe();
};
#endif // ANDROID
#endif // GET_PATH_APPLICATION_H
