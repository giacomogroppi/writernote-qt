#include "datawrite/savefile.h"
#include "dataread/xmlstruct.h"
#include "core/WZipWriter.h"
#include "core/WFile.h"
#include "core/WZipWriter.h"

int savefile::moveFileIntoZip(
        const QByteArray    &from,
        const QByteArray    &path,
        WZipWriter          &writer,
        const QByteArray    &name)
{
    size_t sizeFile = xmlstruct::get_size_file(from);

    QByteArray tmp;

    if(!WFile::readFile(tmp, path.constData())){
        return ERROR;
    }

    if(tmp.size() != (int)sizeFile){
        return ERROR;
    }

    if(writer.write(tmp.constData(), tmp.size(), name))
        return ERROR;
    return OK;
}
