#include "pathfile.h"
#include "../slash/slash.h"



QString pathFile::remove_file(const QString &path)
{
    const char slash = slash::__slash();
    const int indexLast = path.lastIndexOf(slash);

#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
    if(indexLast == -1){
        return changeSlash(path.mid(0, path.lastIndexOf('/')));
    }
#endif
    return path.mid(0, indexLast);
}

QString pathFile::changeSlash(const QString &path){
    QString tmp = path;
    int i, len;
    len = path.length();

#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
    for(i=0; i<len; ++i){
        if(tmp.at(i) == '/')
            tmp.operator[](i) = '\\';
    }
#endif

    return tmp;
}
