#include "FileManager.h"
#include "core/WDir.h"
#include <utility>

FileManager::FileManager(WObject *parent, WByteArray basePath)
    : WObject(parent)
    , _basePath(std::move(basePath))
    , _dir(FileManager::getAllDir(_basePath))
{
}

FileManager::~FileManager() = default;

const WListFast<Directory> &FileManager::getDirectory() const
{
    return this->_dir;
}

auto FileManager::getAllDir(const WByteArray &path) -> WListFast<Directory>
{
    WListFast<Directory> ret = {};
    Directory dir(path);
    const auto directories = dir.allDirsInFolder();

    for (const auto &folder: directories) {
        ret.append(Directory(folder));
    }

    return ret;
}

auto FileManager::getCurrentPath() const -> WString
{
    return _basePath;
}

auto FileManager::moveTo(const WString& newPath) -> void
{
    Directory dir (this->_basePath);
    dir.moveAllFilesTo(newPath);

    this->_basePath = newPath.toUtf8();

    _dir = FileManager::getAllDir(_basePath);
}