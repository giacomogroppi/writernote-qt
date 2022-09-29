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
    QFile file;
    if(!file.open(QFile::ReadOnly))
        return -1;
    W_ASSERT(to.isEmpty());
    to = file.readAll();

    const auto size = file.size();

    file.close();

    if(unlikely(size != to.size()))
        return -1;

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
