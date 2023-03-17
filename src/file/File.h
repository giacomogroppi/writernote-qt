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

    const QByteArray &getName() const;

    /**
     * requires true
     * ensures
     *  \result.size() >= 0 &&
     *  !\result.contains('.') &&
     *  !\result.contains('/')
    */
    QByteArray getExtension() const;
};


