#pragma once


#include "utils/time/current_time.h"
#include "core/WByteArray.h"

class File {
private:
    WByteArray _name;
public:
    /*
     * requires
     *  name.size() > 0 &&
     *  !name.contains('/') &&
     *  !name.lastIndexOf('.') + 1 < name.size()
     * ensures
     *  getName() == name
    */
    explicit File(const WByteArray &name);
    File(const std::string &path);

    ~File() = default;

    enum OpenMode {
        readOnly,
        writeOnly
    };

    bool open(enum OpenMode openMode);
    bool close();

    /*
     * requires true
     * ensures
     *  getName().size() + getExtension().size() == \result.size() &&
     *  (\forall int i; i >= 0 && i < getName().size();
     *      \result.at(i) == getName().at(i)
     *  ) &&
     *  (\forall int i; i >= 0 && i < getExtension().size();
     *      getExtension().at(i) == \result.at(i + getName().size())
     *  )
    */
    const WByteArray &getFullName() const;
    WByteArray getName() const;

    /*
     * requires true
     * ensures
     *  \result.size() >= 0 &&
     *  !\result.contains('.') &&
     *  !\result.contains('/')
    */
    WByteArray getExtension() const;

    /*
     * requires
     *  position.count('.') == 1 &&
     *  position.contains('.') &&
     *  position.lastIndexOf('.') + 1 < position.size()
    */
    static bool createFile(const WByteArray &position);
    const WDate &getLastMod() const;
    bool operator==(const File &other) const;
    static bool exists(const WByteArray &path);
};


