#pragma once


#include "FileContainer.h"

#define W_ZIP_READ 0
#define W_ZIP_WRITE 1

namespace WZipCommon
{
    /**
     * \return < 0 if error
     * */
    int removeFile(const char *fileZip, const char *nameFileInZip);
};

