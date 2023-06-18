#pragma once

#include "FileInstance.h"
#include "core/String/WString.h"

class FileWriter: public FileInstance {
private:
    static constexpr unsigned long dataReservePerTime = 1024;
    WByteArray  _data;
    WString _name;
public:
    explicit FileWriter (WString nameFile);
    FileWriter (FileWriter &&other) noexcept;

    [[nodiscard]] [[deprecated]]
    bool isOk() const;

    WDISABILE_COPY(FileWriter);

    void write (const void *data, size_t size);

    auto getName() const noexcept -> const WString &;
    auto getData() const noexcept -> const WByteArray &;

    void destroy() noexcept;

    friend class FileContainer;
};
