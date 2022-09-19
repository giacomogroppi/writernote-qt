#pragma once

#include "zip.h"

#define W_ZIP_READ 0
#define W_ZIP_WRITE 1

namespace WZipCommon
{
    int removeFile(const char *fileZip, const char *nameFileInZip);
    zip_t *openZip(const char *pos, int flag);
};

