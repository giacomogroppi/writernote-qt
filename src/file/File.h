#pragma once

#include <QByteArray>

class File {
private:
    QByteArray _name;
public:
    /**
     * requires
     *  name.size() > 0 &&
     *  !name.contains('/') &&
     *  !name.lastIndexOf('.') + 1 < name.size()
     * ensures
     *  getName() == name
    */
    explicit File(const QByteArray &name);
    ~File() = default;

    /**
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
    const QByteArray &getFullName() const;
    QByteArray getName() const;

    /**
     * requires true
     * ensures
     *  \result.size() >= 0 &&
     *  !\result.contains('.') &&
     *  !\result.contains('/')
    */
    QByteArray getExtension() const;

    /**
     * requires
     *  position.count('.') == 1 &&
     *  position.contains('.') &&
     *  position.lastIndexOf('.') + 1 < position.size()
    */
    static bool createFile(const QByteArray &position);
};


