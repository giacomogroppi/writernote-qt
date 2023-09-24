#pragma once

#include "File.h"
#include "core/WList.h"
#include "core/ByteArray/WByteArray.h"
#include "core/WFile.h"
#include <filesystem>

class Directory
{
private:
    WListFast<WFile> _files;
    WPath _path;

    static auto getAllFiles(const WPath& path) -> WListFast<WFile>;

public:
    [[deprecated]]
    explicit Directory (const WPath& path);
    Directory (Directory &&other) noexcept = default;
    Directory (const Directory &other) noexcept = delete;
    ~Directory();

    [[nodiscard]]
    auto getFiles() const -> const WListFast<WFile>&;

    /**
     * requires
     *  file in position don't exists
    */
    template <class T>
            requires (std::is_class<T>::value)
    auto addFiles(const WPath &position, const T& writable) -> int;

    [[nodiscard]]
    auto allDirsInFolder() const -> WListFast<WByteArray>;

    auto moveAllFilesTo(const WPath &newPath) -> void;

    [[nodiscard]]
    auto getFolderName() const -> WString;

    auto removeFile(const WString& name) -> int;

    [[deprecated]]
    static auto removeDir(const WByteArray &path) -> int;
    static auto removeDir(const WPath& path) -> int;

    [[deprecated]]
    static auto createDir(const WByteArray &path) -> int;
    static auto createDir(const WPath& path) -> int;

    [[deprecated]]
    static auto exists(const WByteArray &path) -> bool;
    static auto exists(const WPath& path) -> int;

    auto operator==(const Directory &other) const -> bool;
    auto operator!=(const Directory &other) const -> bool;

    auto operator=(const Directory &other) noexcept -> Directory & = delete;
    auto operator=(Directory &&other) noexcept -> Directory & = default;
};

inline bool Directory::operator ==(const Directory &other) const
{
    return _path == other._path;
}

inline bool Directory::operator !=(const Directory &other) const
{
    return !(*this == other);
}

template <class T>
    requires (std::is_class<T>::value)
inline auto Directory::addFiles(const WPath &position, const T& objectToWrite) -> int
{
    if (WFile::exists(position))
        return -1;

    WFile file (position, WFile::WFileWrite);

    if (T::write (file, objectToWrite) < 0)
        return -1;

    file.close();

    this->_files.append(WFile(position));

    return 0;
}

