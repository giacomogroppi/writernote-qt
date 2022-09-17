#include "WZipWriter.h"

WZipWriter::WZipWriter()
{

}

struct zip* WZipWriter::openZip(const char *path)
{
    const int flag = ZIP_CREATE;
    return zip_open(path, flag, nullptr);
}