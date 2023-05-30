#pragma once

#include "core/WByteArray.h"
#include "utils/WCommonScript.h"

class WFile
{
private:
    FILE *fp;
    WByteArray _path;
public:
    explicit WFile(const WByteArray &path, char mode);
    explicit WFile(const WByteArray &path);
    explicit WFile(const std::string &path, char mode);
    explicit WFile(const char *path, char mode);
    ~WFile();

    enum: int {
        WFileReadOnly,
        WFileWrite,
        WFileAppend
    };

    bool open (int openMode);
    bool isValid() const;
    int write(const void *data, size_t size);
    int read (void *to, size_t size);

    bool close();
    size_t size() const;

    static bool exits(const std::string &path);
    static int fileExist(const WByteArray &to);
    static int readFile(WByteArray &to, const char *pathFile);
    static int saveArrIntoFile(const WByteArray &arr, const std::string &path);
    static WFile open(const WByteArray &path, char openMode);
private:
    static size_t sizefp(FILE *fp);
};

inline bool WFile::isValid() const
{
    return !!this->fp;
}

inline size_t WFile::size() const
{
    W_ASSERT(this->fp != nullptr);
    return WFile::sizefp(this->fp);
}

/**
 * Pass "r" to open file in read only mode
 * Pass "w" to open file in write mode
*/
force_inline WFile WFile::open(const WByteArray &path, char openMode)
{
    return WFile {
        path, openMode
    };
}

inline size_t WFile::sizefp(FILE *fp)
{
    W_ASSERT(fp);
    W_ASSERT(ftell(fp) == 0);
    fseek(fp, 0, SEEK_END);
    const auto size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return size;
}
