#include "WFile.h"
#include "utils/WCommonScript.h"
#include <filesystem>
#include <fstream>
#include <utility>
#include "utils/platform.h"

static auto convertToCanonical (char mode) -> const char *
{
    switch (mode) {
    case WFile::Append:
        return "a";
    case WFile::ReadOnly: {
        if constexpr (is_windows)
            return "rb";
        return "r";
    }
    case WFile::Write: {
        if constexpr (is_windows)
            return "wb";
        return "w";
    }
    }
    W_ASSERT(0);
    return nullptr;
}

auto WFile::exists(const std::filesystem::path &path) noexcept -> bool
{
    return std::filesystem::exists(path);
}

WFile::WFile(const WPath &path, char mode)
    : _fp(nullptr)
    , _path(path)
{
    const auto *m = convertToCanonical(mode);
    const auto string = path.operator std::filesystem::path().string();
    _fp = fopen (string.c_str(), m);
}

WFile::~WFile()
{
    if(this->_fp)
        fclose(this->_fp);
    this->_fp = nullptr;
}

auto WFile::open(int openMode) -> bool
{
    W_ASSERT(this->_fp == nullptr);
    this->_fp = fopen(this->_path.operator std::filesystem::path().c_str(), convertToCanonical(openMode));
    return _fp != nullptr;
}

auto WFile::write(const void *data, size_t size) -> Error
{
    W_ASSERT(this->_fp);
    if (size == 0)
        return Error::makeOk();

    const auto res = fwrite(data, size, 1, this->_fp);

    // TODO: find the error
    if (res != 1) {
        return Error::makeCorruption();
    }

    return Error::makeOk();
}

auto WFile::read(void *to, size_t size) const -> Error
{
    W_ASSERT(_fp != nullptr);
    const auto res = fread(to, size, 1, this->_fp);
    if (res < 1)
        return Error::makeCorruption();
    return Error::makeOk();
}

auto WFile::close() -> bool
{
    W_ASSERT(_fp);
    const auto res = fclose(_fp);
    _fp = nullptr;
    return res == 0;
}

auto WFile::readFile(WByteArray &to, const WPath& pathFile) -> int
{
    WFile f (pathFile, WFile::ReadOnly);

    if(f.isValid())
        return -1;

    const auto size = f.size();

    char data[size];

    if (auto err = f.read(data, size)) {
        return -1;
    }

    to = WByteArray(data, size);

    return 0;
}

auto WFile::saveArrIntoFile(const WByteArray &arr, const WPath &path) -> int
{
    WFile file(path, 'w');

    if(!file.isValid())
        return -1;

    if(file.write(arr.constData(), arr.size()))
        return -2;

    return 0;
}

auto WFile::exits(const std::string &path) -> bool
{
    return std::filesystem::exists(path);
}

auto WFile::remove(const WPath& path) -> int
{
    return std::filesystem::remove(path) ? 0 : -1;
}
