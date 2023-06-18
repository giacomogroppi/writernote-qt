//
// Created by Giacomo Groppi on 07/06/23.
//

#ifndef TESTING_QML_FILECONTAINER_H
#define TESTING_QML_FILECONTAINER_H

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

public:
    explicit FileContainer(WString path);

    enum FileContainerOpenMode {
        Read,
        Write
    };

    /**
     * \param nameFile name of the subfile in this file
     * \return a FileFinder for read this file
     * */
    [[nodiscard]]
    auto getFileReader(const WString &nameFile) const noexcept -> FileReader;

    auto addFile(const FileWriter &nameFile) -> int;

    auto closeFileReader (FileReader &file) const -> void;

    [[nodiscard]]
    auto isOpen() const -> bool;

    [[nodiscard]]
    auto sizeOfFile(const WString &path) const -> size_t;

    /**
     * \return false iff it fail
    */
    auto close() -> bool;

    auto remove(const WString &path) -> bool;

    auto load_ver_0(WFile &file, size_t size) noexcept -> int;
};


#endif //TESTING_QML_FILECONTAINER_H
