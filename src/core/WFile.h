#pragma once

#include <utility>

#include "core/ByteArray/WByteArray.h"
#include "utils/WCommonScript.h"
#include "utils/time/current_time.h"
#include "Readable.h"
#include "core/Path/WPath.h"

class WFile final: public ReadableAbstract, public WritableAbstract
{
private:
    FILE *_fp;
    WPath _path;
    WDate lastMod;
public:
    explicit WFile (WPath  path);
    explicit WFile (const WPath& path, char mode);

    ~WFile();

    WFile (const WFile &other) noexcept = default;
    WFile (WFile &&file) noexcept;

    enum: int {
        WFileReadOnly,
        WFileWrite,
        WFileAppend
    };

    /**
     * \return false iff it fail
     * */
    auto open (int openMode) -> bool;

    [[nodiscard]]
    auto isValid() const -> bool;

    /**
     * @return integer &lt 0 in case of error
     * */
    [[nodiscard]]
    auto write(const void *data, size_t size) -> int override;

    /**
     * @return integer &lt 0 in case of error
     * */
    template <class T>
        requires (std::is_arithmetic_v<T> && !std::is_pointer_v<T>)
    auto write(const T& data) -> int;

    /**
     * \return < 0 in case of error
     * */
    auto read (void *to, size_t size) const -> int final;

    auto close() -> bool;
    auto size() const -> size_t;

    static auto exits(const std::string &path) -> bool;

    static auto remove(const WPath& path) -> int;
    static auto readFile(WByteArray &to, const WPath& path) -> int;
    static auto saveArrIntoFile(const WByteArray &arr, const WPath &path) -> int;
    static auto open(const WPath &path, char openMode) -> WFile;

    auto operator==(const WFile &other) const -> bool;
    auto operator!=(const WFile &other) const -> bool;

    static auto exists(const std::filesystem::path &path) noexcept -> bool;

    [[nodiscard]]
    auto getName() const noexcept -> WByteArray;
    auto getLastMod() const noexcept -> WDate;

    auto operator=(WFile &&other) noexcept -> WFile & {
        if (this == &other)
            return *this;

        W_ASSERT(this->_fp == nullptr);

        this->_fp = other._fp;
        this->_path = std::move (other._path);
        this->lastMod = std::move (other.lastMod);

        other._fp = nullptr;
        other._path = "";
        return *this;
    }

private:
    static auto sizefp(FILE *fp) -> size_t;
};

inline auto WFile::isValid() const -> bool
{
    return this->_fp != nullptr;
}

inline auto WFile::size() const -> size_t
{
    W_ASSERT(this->_fp != nullptr);
    return WFile::sizefp(this->_fp);
}

/**
 * Pass "r" to open file in load only mode
 * Pass "w" to open file in write mode
*/
force_inline auto WFile::open(const WPath &path, char openMode) -> WFile
{
    return WFile {
        path, openMode
    };
}

inline auto WFile::sizefp(FILE *fp) -> size_t
{
    W_ASSERT(fp);
    W_ASSERT(ftell(fp) == 0);
    fseek(fp, 0, SEEK_END);
    const auto size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return size;
}

inline auto WFile::getName() const noexcept -> WByteArray
{
    return _path.getNameWithoutExtension();
}

inline auto WFile::getLastMod() const noexcept -> WDate
{
    return this->lastMod;
}

inline auto WFile::operator==(const WFile &other) const -> bool
{
    return this->_path == other._path;
}

inline auto WFile::operator!=(const WFile &other) const -> bool
{
    return !WFile::operator==(other);
}

inline WFile::WFile(WFile &&file) noexcept
    : _fp(file._fp)
    , _path(std::move (file._path))
    , lastMod(file.lastMod)
{
    file._fp = nullptr;
    file._path = "";
}

inline WFile::WFile(WPath path)
    : _fp (nullptr)
    , _path(std::move(path))
    , lastMod()
{
    WDebug(true, "Create a file with name: " << _path);
}

template<class T>
    requires (std::is_arithmetic_v<T> && !std::is_pointer_v<T>)
inline auto WFile::write(const T &data) -> int
{
    return write(&data, sizeof(data));
}

