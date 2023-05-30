#include "Directory.h"
#include "core/WString.h"
#include <filesystem>

Directory::Directory(const WByteArray &path)
    : _files(Directory::getAllFile(path))
    , _path(path)
{
}

Directory::~Directory() = default;

const WList<WFile> &Directory::getFiles() const
{
    return this->_files;
}

bool Directory::addFiles(const WByteArray &position)
{
    W_ASSERT(0);

    this->_files.append(WFile(position));

    return true;
}

WList<WFile> Directory::getAllFile(const WByteArray &path)
{
    WList<WFile> ret = {};
    Directory dir(path);

    for (const auto & entry : std::filesystem::directory_iterator(path.toStdString())) {
        ret.append(WFile(WByteArray(entry.path().c_str())));
    }

    return ret;
}

WList<WByteArray> Directory::allDirsInFolder() const
{
    WList<WByteArray> res {};

    for (const auto& entry : std::as_const(_files)) {
        res.append(entry.getName());
    }

    return res;
}

