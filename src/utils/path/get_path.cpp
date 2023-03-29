#include "get_path.h"
#include "utils/slash/slash.h"
#include "utils/get_path_application.h"

#if defined(unix) || defined(MACOS) || defined(IOS_WRITERNOTE)
#define POS_HOME "HOME"
#elif WIN32
#define POS_HOME "USERPROFILE"
#endif

#define FOLDERNAME ".writernote"
#define NAME_USER "USERNAME" /* saim form linux and windows */
#include <QDir>

static bool createTempFolder(const QString &path);

static bool createTempFolder(const QString &path){
    if(!QDir(path).exists()){
        return QDir().mkdir(path);
    }

    return true;
}

/*
 * the function does not check if the
 * folder to save the audio exists
*/
QString get_path_no_controll()
{
    return get_path_application::exe();
}
