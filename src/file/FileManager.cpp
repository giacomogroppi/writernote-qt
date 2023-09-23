#include "FileManager.h"
#include "core/WDir.h"
#include <utility>

FileManager::FileManager(WObject *parent, WByteArray basePath, bool createDir)
    : WObject(parent)
    , _basePath(std::move(basePath))
    , _dir(FileManager::getAllDir(_basePath))
{
    if (createDir and _dir.isEmpty()) {
        // maybe basePath don't exist
        Directory::createDir(_basePath);
    }
}

FileManager::~FileManager() = default;

// TODO: move it to .h file
auto FileManager::getDirectory() const -> const WListFast<Directory> &
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