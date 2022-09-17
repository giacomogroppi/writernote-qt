#include "WZipWriterSingle.h"
#include "WZipWriter.h"

WZipWriterSingle::~WZipWriterSingle()
{
    if(_allocated)
        WFree(this->_data);
}

int WZipWriterSingle::commit_change(const QByteArray &zipName, const QByteArray &nameFileInZip)
{
    WZipWriter writer;

    W_ASSERT(this->_allocated);

    if(writer.init(zipName.constData()))
        return -1;
    if(writer.write(this->_data, this->_max, nameFileInZip.constData()))
        return -1;
    return 0;
}
