#include "File.h"
#include <utility>
#include "utils/WCommonScript.h"
#include "utils/slash/slash.h"
#include <stdlib.h>
#include <stdio.h>
#include <QFile>

File::File(const QByteArray &name)
    : _name(name)
{
    W_ASSERT(!name.contains(slash::__slash()));
}

const QByteArray &File::getFullName() const
{
    return this->_name;
}

QByteArray File::getName() const
{
    QByteArray ret {};
    for (const auto c : qAsConst(this->_name)) {
        if (c == '.') {
            return ret;
        }
        ret.append(c);
    }
    W_ASSERT(0);
    return {};
}

QByteArray File::getExtension() const
{
    QByteArray e = {};
    auto i = this->_name.size();

    for (i --; i >= 0; i --) {
        if (this->_name.at(i) == '.') {
            W_ASSERT(!e.contains('.'));
            W_ASSERT(!e.contains('/'));
            return e;
        }

        e.append(_name.at(i));
    }

    W_ASSERT(0);
    return {};
}

bool File::createFile(const QByteArray &position)
{
    W_ASSERT(position.count('.') == 1);
    W_ASSERT(position.indexOf('.') + 1 < position.size());
    QFile file(position);

    /** file already exists */
    if (file.open(QFile::ReadOnly)) {
        return false;
    }

    /** we were unable to open the file in writemode*/
    if (!file.open(QFile::WriteOnly)) {
        return false;
    }

    file.close();
    return true;
}

const QDate &File::getLastMod() const
{
    qWarning() << "Function not implemented";
    return QDate();
}

bool File::operator==(const File &other) const
{
    if (this == &other)
        return true;
    return this->getFullName() == other.getFullName();
}
