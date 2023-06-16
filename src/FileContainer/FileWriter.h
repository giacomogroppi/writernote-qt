//
// Created by Giacomo Groppi on 07/06/23.
//

#ifndef TESTING_QML_FILEWRITER_H
#define TESTING_QML_FILEWRITER_H

#include "FileInstance.h"
#include "core/String/WString.h"

class FileWriter: public FileInstance {
private:
    static constexpr auto dataReservePerTime = 2048;
    WByteArray  _data;
    WString _name;
public:
    explicit FileWriter (WString nameFile);
    FileWriter (FileWriter &&other) noexcept;

    [[nodiscard]] [[deprecated]]
    bool isOk() const;

    WDISABILE_COPY(FileWriter);

    void write (const void *data, size_t size);

    void destroy() noexcept;
};


#endif //TESTING_QML_FILEWRITER_H
