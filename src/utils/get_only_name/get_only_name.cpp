#include "get_only_name.h"
#include "utils/common_def.h"
#include "utils/slash/slash.h"
#include "../path/pathfile.h"
#include "core/String/WString.h"
#include "core/WListFast.h"

WString get_only_name::exe(const WString &path)
{
    const char slash = slash::__slash();

#if defined(WIN64) || defined(WIN32)
    WString tmp;
    const int index_slash = path.indexOf(slash);
    if(index_slash == -1)
        tmp = pathFile::changeSlash(path);
    const WListFastFast<WString> list = tmp.split('\\');
#else
    const auto list = path.split(slash);
#endif

    if(!list.size())
        return "";

    int index = list.last().lastIndexOf("." APP_EXT);
    return list.last().mid(0, index);
}
