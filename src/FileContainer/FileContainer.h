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
    using Pair = WPair<WString, SharedPtr<WByteArray>>;

    WString _path;
    WListFast<Pair> _subFiles;
    bool _isOk;

public:
    FileContainer(WString path);

    enum FileContainerOpenMode {
        Read,
        Write
    };

    /**
     * \param nameFile name of the subfile in this file
     * \return a FileFinder for read this file
     * */
    FileReader &getFileReader(const WString &nameFile);
    int addFile(const FileWriter &nameFile);

    void closeFileReader (FileReader &file) const;

    [[nodiscard]]
    bool isOpen() const;

    [[nodiscard]]
    size_t sizeOfFile(const WString &path) const;

    /**
     * \return false iff it fail
    */
    bool close();

    bool remove(const WString &path);

    int load_ver_0(WFile &file, size_t size) noexcept;
};


#endif //TESTING_QML_FILECONTAINER_H
