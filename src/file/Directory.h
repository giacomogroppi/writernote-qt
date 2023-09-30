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

    template <class T> requires (std::is_class<T>::value)
    auto addFiles(const WPath &position, const T& objectToWrite) -> int;

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

template <class T>
    requires (std::is_class<T>::value)
inline auto Directory::addFiles(const WPath &position, const T& objectToWrite) -> int
{
    const auto exists = WFile::exists(position);

    WFile file (position, WFile::WFileWrite);

    if (T::write (file, objectToWrite) < 0)
        return -1;

    file.close();

    if (not exists)
        this->_files.append(WFile(position));

    return 0;
}

