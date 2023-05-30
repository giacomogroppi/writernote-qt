#include "permission.h"

#include "file/Directory.h"
#include "file/File.h"

bool permission::open(const char *file, e_permission request)
{
    WByteArray path(file, strlen(file));
    if(request == e_permission::exist){
        return File::exists(path);
    }

    WFile file_temp(path);

    if (request == e_permission::readOnly) {
        if (!file_temp.open(WFile::WFileReadOnly))
            return false;
        file_temp.close();
        return true;
    }


    if (request == e_permission::writeOnly) {
        if (!file_temp.open(WFile::WFileWrite))
            return false;
        file_temp.close();
        return true;
    }

    return false;
}

