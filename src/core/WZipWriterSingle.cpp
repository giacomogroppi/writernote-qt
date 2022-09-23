#include "WZipWriterSingle.h"
#include "WZipWriter.h"

WZipWriterSingle::~WZipWriterSingle()
{
    if(_allocated)
        WFree(this->_data);
}

void WZipWriterSingle::write_string(const char *str, int size)
{
    static_assert(sizeof(size) == 4);
    this->write_object(size);
    if(size)
        this->write(str, size);
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
