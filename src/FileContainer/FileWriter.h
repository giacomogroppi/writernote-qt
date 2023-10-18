#pragma once

#include "FileInstance.h"
#include "core/String/WString.h"

class FileWriter final: public FileInstance, public WritableAbstract
{
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

    auto write (const void *data, size_t size) -> Error final;

    nd auto getName() const noexcept -> const WString &;
    nd auto getData() const noexcept -> const WByteArray &;

    void destroy() noexcept;

    friend class FileContainer;
};
