#include "pathfile.h"
#include "../slash/slash.h"

QString pathFile::remove_file(const QString &path)
{
    const char slash = slash::__slash();
    const int indexLast = path.lastIndexOf(slash);

#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
    return changeSlash(path.mid(0, path.lastIndexOf('/')));
#else
    return path.mid(0, indexLast);
#endif
}

#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
QString pathFile::changeSlash(const QString &path){
    QString tmp = path;
    int i;
    const int len = path.length();
    for(i=0; i<len; ++i){
        if(tmp.at(i) == '/')
            tmp.operator[](i) = '\\';
    }

    return tmp;
}
#endif
