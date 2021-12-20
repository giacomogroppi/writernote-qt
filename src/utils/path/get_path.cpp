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

/*
 * warning:
 * the pointer returning the function will be
 * deleted at the end of the call
 *
 * for audio -> if it fail to create temp
 * folder -> return NULL
*/

QString get_path(path::e_path var)
{
    QString tmp;
    tmp = get_path_no_controll();
    if(var == path::audio_pos){
        if(!createTempFolder(tmp))
            return "";

        return tmp;
    }

    if(var == path::log){
        tmp += "writernote-log";
        if(!createTempFolder(tmp))
            return "";
        return tmp;
    }

    if(var == path::tmp_file_not_save){
        if(!createTempFolder(tmp))
            return "";
        return tmp;
    }

    if(var == path::home){
        return (QString)getenv(POS_HOME);
    }
    if(var == path::nameuser){
        return getenv(NAME_USER);
    }

    return "";
}

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
QString get_path_no_controll(){
    return get_path_application::exe();

    /*QString tmp = getenv(POS_HOME);
    const char slash = slash::__slash();

    tmp += slash;
    tmp += FOLDERNAME;

    tmp += slash;
    return tmp;*/
}
