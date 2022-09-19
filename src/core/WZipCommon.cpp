#include "wzipcommon.h"
#include "utils/common_script.h"

int WZipCommon::removeFile(const char *fileZip, const char *nameFileInZip)
{
    struct zip_stat stat;
    zip_t *Zip = WZipCommon::openZip(fileZip, W_ZIP_WRITE);

    if(unlikely(!Zip))
        return -1;

    zip_stat_init(&stat);
    zip_stat(Zip, nameFileInZip, 0, &stat);
    const auto res = zip_delete(Zip, stat.index);
    zip_close(Zip);

    if(res < 0)
        return -1;

    return 0;
}

zip_t *WZipCommon::openZip(const char *pos, int mode)
{
    int flag;

    W_ASSERT(mode == W_ZIP_READ or mode == W_ZIP_WRITE);

    if(mode == W_ZIP_READ)
        flag = ZIP_RDONLY;
    else{
        flag = ZIP_CREATE;
    }

    return zip_open(pos, flag, nullptr);
}
