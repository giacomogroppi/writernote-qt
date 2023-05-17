#include "datawrite/savefile.h"
#include "dataread/xmlstruct.h"
#include "core/WZipWriter.h"
#include "core/WFile.h"
#include "core/WZipWriter.h"

int savefile::moveFileIntoZip(
        const WByteArray    &pathFile,
        WZipWriter          &writer,
        const WByteArray    &nameInZip)
{
    WByteArray tmp;

    if(WFile::readFile(tmp, pathFile) < 0){
        return ERROR;
    }

    W_ASSERT(static_cast<size_t>(tmp.size()) == xmlstruct::get_size_file(pathFile));

    if(writer.write(tmp.constData(), tmp.size(), nameInZip))
        return ERROR;

    return OK;
}
