//
// Created by Giacomo Groppi on 07/06/23.
//

#ifndef TESTING_QML_FILECONTAINER_H
#define TESTING_QML_FILECONTAINER_H

#include "WString.h"
#include "FileReader.h"

class FileContainer {
private:
    WString _path;
public:
    FileContainer(const WString &path);

    enum FileContainerOpenMode {
        Read,
        Write
    };

    /**
     * \param nameFile name of the subfile in this file
     * \return a FileFinder for read this file
     * */
    FileReader &getFileReader(const WString &nameFile);


    [[nodiscard]]
    bool isOpen() const;

    [[nodiscard]]
    size_t sizeOfFile(const WString &path) const;

    bool close();

    bool remove(const WString &path);
};


#endif //TESTING_QML_FILECONTAINER_H
