#pragma once


#include "utils/time/current_time.h"
#include "core/WByteArray.h"

class File {
private:
    WByteArray _name;
public:
    explicit File(const WByteArray &name);

    File();
    ~File() = default;

    enum OpenMode: int {
        readOnly = 0x0,
        writeOnly = 0x1,
        append = 0x2
    };

    bool open(int openMode);
    int write(const WByteArray &data);
    bool close();

    size_t size() const;

    const WByteArray &getFullName() const;
    WByteArray getName() const;

    WByteArray getExtension() const;

    static bool createFile(const WByteArray &position);
    const WDate &getLastMod() const;
    bool operator==(const File &other) const;
    static bool exists(const WByteArray &path);

    constexpr bool isOpen() const noexcept;
};


