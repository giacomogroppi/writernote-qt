#pragma once

#include <QByteArray>
#include "utils/common_script.h"

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

    static int fileExist(const QByteArray &to);
    static int readFile(QByteArray &to, const char *pathFile);
    static int saveArrIntoFile(const QByteArray &arr, const QString &path);
    static FILE *open(const QByteArray &path, const char *flat);
    static size_t size(FILE *fp);
};

inline bool WFile::isValid() const
{
    return !!this->fp;
}

/**
 * Pass "r" to open file in read only mode
 * Pass "w" to open file in write mode
*/
force_inline FILE *WFile::open(const QByteArray &path, const char *flag)
{
    W_ASSERT(WStrEqual(flag, "r") == true or WStrEqual(flag, "w") == true);

#if defined(WIN32) || defined(WIN64)
    FILE *fp;
    if(*flag == 'r'){
        fp = fopen(path.constData(), "rb");
    }else{
        fp = fopen(path.constData(), "wb");
    }

    return fp;
#else
    return fopen(path.constData(), flag);
#endif
}

inline size_t WFile::size(FILE *fp)
{
    W_ASSERT(fp);
    W_ASSERT(ftell(fp) == 0);
    fseek(fp, 0, SEEK_END);
    const auto size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return size;
}
