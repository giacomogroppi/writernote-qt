#include "Directory.h"
#include "core/WString.h"
#include <filesystem>

Directory::Directory(const WByteArray &path)
    : _files(Directory::getAllFile(path))
    , _path(path)
{
}

Directory::~Directory() = default;

const WList<File> &Directory::getFiles() const
{
    return this->_files;
}

bool Directory::addFiles(const WByteArray &position)
{
    if (!File::createFile(position)) {
        return false;
    }

    this->_files.append(File(position));

    return true;
}

WList<File> Directory::getAllFile(const WByteArray &path)
{
    WList<File> ret = {};
    Directory dir(path);

    for (const auto & entry : std::filesystem::directory_iterator(path.toStdString())) {
        ret.append(File(WByteArray(entry.path().c_str())));
    }

    return ret;
}

