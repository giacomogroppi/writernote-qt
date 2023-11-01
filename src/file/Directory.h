#pragma once

#include "File.h"
#include "core/WList.h"
#include "core/ByteArray/WByteArray.h"
#include "core/WFile.h"
#include <filesystem>

class Directory
{
private:
    static constexpr auto debug = false;

    WListFast<WFile> _files;
    WPath _path;

    static auto getAllFiles(const WPath& path) -> WListFast<WFile>;

public:
    /**
     * \brief Constructor of the class with a given path. This method doesn't create the folder in case
     *  it does not exist
     * \param path The path the the folder
     * */
    explicit Directory (const WPath& path);

    Directory (Directory &&other) noexcept = default;
    Directory (const Directory &other) noexcept = delete;
    ~Directory();

    /**
     * \brief Getter
     * \return The list of files [open or close] in the folder
     * */
    nd auto getFiles() const -> const WListFast<WFile>&;

    auto addFiles(const WString &nameFile, const Fn<int(WritableAbstract& readable)>& methodWrite) -> int;

    nd auto allDirsInFolder() const -> WListFast<WByteArray>;

    auto moveAllFilesTo(const WPath &newPath) -> void;

    nd auto getFolderName() const -> WString;

    nd auto removeFile(const WString& name) -> int;

    nd static auto removeDir(const WPath& path) -> int;

    nd static auto createDir(const WPath& path) -> int;

    nd static auto exists(const WPath& path) -> bool;

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

inline auto Directory::addFiles(const WString &nameFile, const Fn<int(WritableAbstract&)>& methodWrite) -> int
{
    const auto path = this->_path
                      / nameFile;

    const auto exists = WFile::exists(path);

    WFile file (path, WFile::Write);

    if (not file.isValid())
        return -1;

    if (methodWrite(file) < 0) {
        file.close();
        WFile::remove(path);
        return -1;
    }

    file.close();

    if (not exists)
        this->_files.append(WFile(path));

    return 0;
}

