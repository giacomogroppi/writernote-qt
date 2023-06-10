//
// Created by Giacomo Groppi on 07/06/23.
//

#ifndef TESTING_QML_FILECONTAINER_H
#define TESTING_QML_FILECONTAINER_H

#include "core/WString.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "core/WListFast.h"

class FileContainer {
private:
    using pair = std::pair<WString, std::shared_ptr<WByteArray>>;


    WString _path;
    WListFast<pair> _subFiles;
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

    bool close();

    bool remove(const WString &path);

    void load_ver_0(const void *string, size_t size) noexcept;
};


#endif //TESTING_QML_FILECONTAINER_H
