//
// Created by Giacomo Groppi on 07/06/23.
//

#ifndef TESTING_QML_FILEWRITER_H
#define TESTING_QML_FILEWRITER_H

#include "FileInstance.h"
#include "core/String/WString.h"

class FileWriter: public FileInstance {
public:
    FileWriter (const WString &nameFile);

    [[nodiscard]]
    bool isOk() const;

    [[nodiscard]]
    size_t write (const void *data, size_t size);

    void destroy() noexcept;
};


#endif //TESTING_QML_FILEWRITER_H
