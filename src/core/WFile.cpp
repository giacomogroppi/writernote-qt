#include "WFile.h"
#include "utils/common_script.h"
#include <QFile>

WFile::WFile(const QByteArray &path, const char mode)
{
    char tmp[2] = {mode, '\0'};
    fp = fopen(path.constData(), tmp);
}

WFile::WFile(const QString &path, const char mode):
    WFile(path.toUtf8(), mode)
{
}

WFile::WFile(const char *path, const char mode):
    WFile(QByteArray(path), mode)
{
}

WFile::~WFile()
{
    if(this->fp)
        fclose(this->fp);
}

int WFile::write(const void *data, size_t size)
{
    W_ASSERT(this->fp);
    const auto res = fwrite(data, size, 1, this->fp);

    if(unlikely(res != 1))
        return -1;

    return 0;
}

int WFile::fileExist(const QByteArray &to)
{
    if(QFile::exists(to))
        return 0;
    return -1;
}

int WFile::readFile(QByteArray &to, const char *pathFile)
{
    FILE *fp = WFile::open(pathFile, "r");

    if(!fp)
        return -1;

    const auto size = WFile::size(fp);

    char data[size];

    if(fread(data, size, 1, fp) < 1)
        return -1;

    to = QByteArray(data, size);

    return 0;
}

int WFile::saveArrIntoFile(const QByteArray &arr, const QString &path)
{
    WFile file(path, 'w');

    if(!file.isValid())
        return -1;

    if(file.write(arr.constData(), arr.size()) < 0)
        return -2;

    return 0;
}
