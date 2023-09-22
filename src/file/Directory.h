#pragma once

#include "File.h"
#include "core/WList.h"
#include "core/ByteArray/WByteArray.h"
#include "core/WFile.h"

class Directory
{
private:
    WListFast<WFile> _files;
    WByteArray _path;

    static auto getAllFile(const WByteArray &path) -> WListFast<WFile>;
public:
    explicit Directory (const WByteArray &path);
    Directory (Directory &&other) noexcept = default;
    Directory (const Directory &other) noexcept = delete;
    ~Directory();

    auto getFiles() const -> const WListFast<WFile>&;

    /**
     * requires
     *  file in position don't exists
    */
    auto addFiles(const WByteArray &position) -> bool;

    auto allDirsInFolder() const -> WListFast<WByteArray>;

    auto moveAllFilesTo(const WString &newPath) -> void;

    auto getFolderName() const -> WString;

    static int removeDir(const WByteArray &path);
    static int createDir(const WByteArray &path);
    static bool exists(const WByteArray &path);

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



