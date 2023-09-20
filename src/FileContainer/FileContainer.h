#pragma once

#include "core/String/WString.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "core/WListFast.h"
#include "core/WFile.h"
#include "core/WPair.h"
#include "core/pointer/SharedPtr.h"

class FileContainer {
private:
    using Pair = WPair<
                WString,
                SharedPtr<WByteArray>
            >;

    WString _path;
    WListFast<Pair> _subFiles;
    bool _isOk;
    WFile _file;

    auto dataOf (const WString &name) const -> SharedPtr<WByteArray>;

public:
    explicit FileContainer(WString path);

    enum FileContainerOpenMode {
        Read,
        Write
    };

    /**
     * \param nameFile name of the subfile in this file
     * \return a FileFinder for load this file
     * */
    [[nodiscard]]
    auto getFileReader(const WString &nameFile) const noexcept -> FileReader;

    /**
     * \param file File to be added
     * \return &lt 0 in case of error
     * */
    auto addFile(const FileWriter &file) -> int;

    /**
     * \param file File to be added
     * \return &lt 0 in case of error
     * */
    auto addFile(FileWriter &&file) -> int;

    /**
     * \param file File to be added
     * \return &lt 0 in case of error
     * */
    auto addFile(WString name, WByteArray data) -> int;

    auto closeFileReader (FileReader &file) const -> void;

    [[nodiscard]]
    auto isOpen() const -> bool;

    [[nodiscard]]
    auto sizeOfFile(const WString &path) const -> size_t;

    /**
     * \return false iff it fail
    */
    auto close() -> bool;

    /**
     * return True in case the file was load correctly
     * */
    auto isOk() const -> bool;

    auto remove(const WString &path) -> bool;

    auto load_ver_0(WFile &file, size_t size) noexcept -> int;
};

inline auto FileContainer::closeFileReader(FileReader &file) const -> void
{
    /// for now it's not necessary to do anything at all to close a FileReader
    unused(file);
}

inline auto FileContainer::isOpen() const -> bool
{
    return this->_file.isValid();
}
