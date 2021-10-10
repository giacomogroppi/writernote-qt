#include "pathfile.h"
#include "../slash/slash.h"

QString pathFile::remove_file(const QString &path)
{
    const char slash = slash::__slash();
    int indexLast = path.lastIndexOf(slash);

#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
    if(indexLast != -1)
        return path.mid(0, indexLast);

    const QString &ref = pathFile::changeSlash(path);
    indexLast = ref.lastIndexOf(slash);
    return ref.mid(0, indexLast);
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
