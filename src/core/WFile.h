#pragma once

#include <QByteArray>

class WFile
{
private:
    FILE *fp;
public:
    WFile(const QByteArray &path, const char mode);
    WFile(const QString &path, const char mode);
    WFile(const char *path, const char mode);
    ~WFile();

    bool isValid() const;
    int write(const void *data, size_t size);

    static int readFile(QByteArray &to, const char *pathFile);
    static int saveArrIntoFile(const QByteArray &arr, const QString &path);
};

inline bool WFile::isValid() const
{
    return !!this->fp;
}
