#include "File.h"
#include <utility>
#include "utils/WCommonScript.h"
#include "utils/slash/slash.h"

File::File(const QByteArray &name)
    : _name(name)
{
    W_ASSERT(!name.contains(slash::__slash()));
}

const QByteArray &File::getName() const
{
    return this->_name;
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
