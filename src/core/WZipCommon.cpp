#include "WZipCommon.h"
#include "utils/WCommonScript.h"
#include "core/FileContainer.h"

int WZipCommon::removeFile(const char *fileZip, const char *nameFileInZip)
{
    FileContainer container (fileZip);
    if (!container.remove(WString(nameFileInZip)))
        return -1;
    return 0;
}
