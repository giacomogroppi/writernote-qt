#include "WFile.h"
#include "utils/WCommonScript.h"
#include <filesystem>
#include <fstream>
#include <utility>
#include "utils/platform.h"

static auto convertToCanonical (char mode) -> const char *
{
    switch (mode) {
    case WFile::WFileAppend:
        return "a";
    case WFile::WFileReadOnly: {
        if constexpr (is_windows)
            return "rb";
        return "r";
    }
    case WFile::WFileWrite: {
        if constexpr (is_windows)
            return "wb";
        return "w";
    }
    }
    W_ASSERT(0);
    return nullptr;
}

WFile::WFile(WByteArray path)
        : _fp(nullptr)
        , _path(std::move(path))
        , lastMod()
{

}

auto WFile::exists(const WByteArray& array) noexcept -> bool
{
    return std::filesystem::exists(array.toStdString());
}

auto WFile::exists(const std::filesystem::path &path) noexcept -> bool
{
    return std::filesystem::exists(path);
}

WFile::WFile(const std::filesystem::path &path, char mode)
    : _fp(nullptr)
    , _path(path.string().c_str())
{
    const auto *m = convertToCanonical(mode);
    _fp = fopen (path.c_str(), m);
}

WFile::WFile(const WByteArray &path, const char mode)
    : _fp(nullptr)
    , _path(path)
{
    const auto *m = convertToCanonical(mode);

    _fp = fopen(path.constData(), m);
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
    if(this->_fp)
        fclose(this->_fp);
}

auto WFile::open(int openMode) -> bool
{
    W_ASSERT(this->_fp == nullptr);
    this->_fp = fopen(this->_path.constData(), convertToCanonical(openMode));
    return _fp != nullptr;
}

auto WFile::write(const void *data, size_t size) -> int
{
    W_ASSERT(this->_fp);
    const auto res = fwrite(data, size, 1, this->_fp);
    //std::ofstream stream("ciao", std::ios_base::binary)

    if(un(res != 1))
        return -1;

    return 0;
}

auto WFile::read(void *to, size_t size) const -> int
{
    W_ASSERT(_fp != nullptr);
    const auto res = fread(to, size, 1, this->_fp);
    return res < 1;
}

auto WFile::close() -> bool
{
    W_ASSERT(_fp);
    const auto res = fclose(_fp);
    return res == 0;
}

auto WFile::fileExist(const WByteArray &to) -> int
{
    using namespace std;
    using namespace std::filesystem;

    if (exits(to.toStdString()))
        return 0;
    return -1;
}

auto WFile::readFile(WByteArray &to, const char *pathFile) -> int
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

auto WFile::saveArrIntoFile(const WByteArray &arr, const std::string &path) -> int
{
    WFile file(path, 'w');

    if(!file.isValid())
        return -1;

    if(file.write(arr.constData(), arr.size()) < 0)
        return -2;

    return 0;
}

auto WFile::exits(const std::string &path) -> bool
{
    return std::filesystem::exists(path);
}
