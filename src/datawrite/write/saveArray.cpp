#include "datawrite/savefile.h"
#include "dataread/xmlstruct.h"
#include "FileContainer/WZipWriter.h"
#include "core/WFile.h"
#include "FileContainer/WZipWriter.h"

auto savefile::moveFileIntoZip(
        const WPath         &pathFile,
        WZipWriter          &writer,
        const WByteArray    &nameInZip) -> int
{
    WByteArray tmp;

    if (WFile::readFile(tmp, pathFile) < 0) {
        return ERROR;
    }

    W_ASSERT(static_cast<size_t>(tmp.size()) == xmlstruct::get_size_file(pathFile));

    if(writer.write(tmp.constData(), tmp.size(), nameInZip.constData()))
        return ERROR;

    return OK;
}
