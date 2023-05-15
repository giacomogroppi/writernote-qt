#include "File.h"
#include <utility>
#include "utils/WCommonScript.h"
#include "utils/slash/slash.h"
#include <stdlib.h>
#include <stdio.h>
#include <filesystem>
#include <fstream>

File::File(const WByteArray &name)
    : _name(name)
{
    W_ASSERT(!name.contains(slash::__slash()));
}

const WByteArray &File::getFullName() const
{
    return this->_name;
}

WByteArray File::getName() const
{
    WByteArray ret {};
    for (const auto c : std::as_const(this->_name)) {
        if (c == '.') {
            return ret;
        }
        ret.append(c);
    }
    W_ASSERT(0);
    return {};
}

WByteArray File::getExtension() const
{
    WByteArray e = {};
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

bool File::createFile(const WByteArray &position)
{
    W_ASSERT(position.count('.') == 1);
    W_ASSERT(position.indexOf('.') + 1 < position.size());

    if (exists(position))
        return false;

    std::ofstream output_file("output.txt");

    // verifica se il file è stato creato correttamente
    if (!output_file.is_open()) {
        std::cerr << "Errore: impossibile creare il file" << std::endl;
        return false;
    }

    // chiudere il file di output
    output_file.close();

    return true;
}

const WDate &File::getLastMod() const
{
    W_ASSERT(0);
    return WDate();
}

bool File::operator==(const File &other) const
{
    if (this == &other)
        return true;
    return this->getFullName() == other.getFullName();
}
