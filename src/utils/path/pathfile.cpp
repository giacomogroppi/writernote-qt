#include "pathfile.h"
#include "utils/slash/slash.h"

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
