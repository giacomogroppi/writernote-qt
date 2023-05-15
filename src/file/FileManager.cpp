#include "FileManager.h"
#include "core/WDir.h"
#include <utility>

FileManager::FileManager(WObject *parent, WByteArray basePath)
    : WObject(parent)
    , _basePath(basePath)
    , _dir(FileManager::getAllDir(_basePath))
{
}

FileManager::~FileManager() = default;

const WList<Directory> &FileManager::getDirectory() const
{
    return this->_dir;
}

WList<Directory> FileManager::getAllDir(const WByteArray &path)
{
    WList<Directory> ret = {};
    Directory dir(path);
    const auto directories = dir.allDirsInFolder();

    for (const auto &folder: directories) {
        ret.append(Directory(folder));
    }

    return ret;
}

