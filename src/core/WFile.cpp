#include "WFile.h"
#include "utils/WCommonScript.h"
#include <filesystem>
#include <fstream>
#include "utils/platform.h"

static const char *convertToCanonical (char mode)
{
    switch (mode) {
    case WFile::WFileAppend:
        return "a";
    case WFile::WFileReadOnly: {
        if (is_windows)
            return "rb";
        return "r";
    }
    case WFile::WFileWrite: {
        if (is_windows)
            return "wb";
        return "w";
    }
    }
    W_ASSERT(0);
    return nullptr;
}

WFile::WFile(const WByteArray &path, const char mode)
{
    const auto *m = convertToCanonical(mode);

    fp = fopen(path.constData(), m);
}

WFile::WFile(const std::string &path, const char mode):
    WFile(path.c_str(), mode)
{
}

WFile::WFile(const char *path, const char mode):
    WFile(WByteArray(path), mode)
{
}

WFile::~WFile()
{
    if(this->fp)
        fclose(this->fp);
}

bool WFile::open(int openMode)
{
    W_ASSERT(this->fp == nullptr);
    this->fp = fopen(this->_path.constData(), convertToCanonical(openMode));
    return fp != nullptr;
}

int WFile::write(const void *data, size_t size)
{
    W_ASSERT(this->fp);
    const auto res = fwrite(data, size, 1, this->fp);
    //std::ofstream stream("ciao", std::ios_base::binary)

    if(un(res != 1))
        return -1;

    return 0;
}

int WFile::read(void *to, size_t size)
{
    W_ASSERT(fp != nullptr);
    const auto res = fread(to, size, 1, this->fp);
    return res < 1;
}

bool WFile::close()
{
    W_ASSERT(fp);
    const auto res = fclose(fp);
    return res == 0;
}

int WFile::fileExist(const WByteArray &to)
{
    using namespace std;
    using namespace std::filesystem;

    if (exits(to.toStdString()))
        return 0;
    return -1;
}

int WFile::readFile(WByteArray &to, const char *pathFile)
{
    WFile f (pathFile, WFile::WFileReadOnly);

    if(f.isValid())
        return -1;

    const auto size = f.size();

    char data[size];

    if (f.read(data, size) < 0) {
        return -1;
    }

    to = WByteArray(data, size);

    return 0;
}

int WFile::saveArrIntoFile(const WByteArray &arr, const std::string &path)
{
    WFile file(path, 'w');

    if(!file.isValid())
        return -1;

    if(file.write(arr.constData(), arr.size()) < 0)
        return -2;

    return 0;
}
