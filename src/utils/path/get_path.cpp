#include "get_path.h"
#include "../slash/slash.h"

#if defined(unix) || defined(MACOS)
#define POS_HOME "HOME"
#elif WIN32
#define POS_HOME "USERPROFILE"
#endif

#define PLUSS(x) x+=slash::__slash()
#define FOLDERNAME ".writernote"
#define NAME_USER "USERNAME" /* saim form linux and windows */
#include <QDir>

static bool createTempFolder(QString &path);

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
    if(var == path::audio_pos){
        tmp = get_path_no_controll();

        if(!createTempFolder(tmp))
            return "";

        return tmp;
    }

    if(var == path::log){
        tmp = get_path_no_controll();
        tmp += "writernote-log";
        if(!createTempFolder(tmp))
            return "";
        return tmp;
    }

    if(var == path::tmp_file_not_save){
        tmp = get_path_no_controll();
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

static bool createTempFolder(QString &path){
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
    QString temp = getenv(POS_HOME);

    PLUSS(temp);
    temp += FOLDERNAME;

    PLUSS(temp);
    return temp;
}
