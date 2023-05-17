#include "get_path.h"
#include "utils/slash/slash.h"
#include "utils/get_path_application.h"
#include "core/WDir.h"
#include "file/Directory.h"

#if defined(unix) || defined(MACOS) || defined(IOS_WRITERNOTE)
#define POS_HOME "HOME"
#elif WIN32
#define POS_HOME "USERPROFILE"
#endif

#define FOLDERNAME ".writernote"
#define NAME_USER "USERNAME" /* saim form linux and windows */

static bool createTempFolder(const WString &path);

static bool createTempFolder(const WString &path){
    if(!WDir::exists(path.toUtf8())){
        return WDir::createDir(path.toUtf8());
    }

    return true;
}

/*
 * the function does not check if the
 * folder to save the audio exists
*/
WString get_path_no_controll()
{
    return get_path_application::exe();
}
