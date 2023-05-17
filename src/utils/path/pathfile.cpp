#include "pathfile.h"
#include "utils/slash/slash.h"

WString pathFile::remove_file(const WString &path)
{
    const char slash = slash::__slash();
    int indexLast = path.lastIndexOf(slash);

#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
    if(indexLast != -1)
        return path.mid(0, indexLast);

    const WString &ref = pathFile::changeSlash(path);
    indexLast = ref.lastIndexOf(slash);
    return ref.mid(0, indexLast);
#else
    return path.mid(0, indexLast);
#endif
}

#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
WString pathFile::changeSlash(const WString &path){
    WString tmp = path;
    int i;
    const int len = path.length();
    for(i=0; i<len; ++i){
        if(tmp.at(i) == '/')
            tmp.operator[](i) = '\\';
    }

    return tmp;
}
#endif
