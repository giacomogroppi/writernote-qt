#include "FileManager.h"
#include "core/WDir.h"
#include <utility>
#include <filesystem>

static const WListFast<WFile> emptyFiles;

FileManager::FileManager(WObject *parent, WByteArray basePath, bool createDir)
    : WObject(parent)
    , _basePath(std::move(basePath))
    , _dir(FileManager::getAllDir(_basePath))
    , _selected(0)
{
    if (createDir and _dir.isEmpty()) {
        // maybe basePath don't exist
        Directory::createDir(_basePath);
    }

    if (_dir.isEmpty() and createDir) {
        Directory::createDir(std::filesystem::path(_basePath.constData()) / "Not classified");
    }
}

FileManager::~FileManager() = default;

// TODO: move it to .h file
auto FileManager::getDirectory() const -> const WListFast<Directory> &
{
    return this->_dir;
}

auto FileManager::getCurrentFiles() const -> const WListFast<WFile>&
{
    if (_dir.isEmpty())
        return emptyFiles;
    return _dir[_selected].getFiles();
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

auto FileManager::removeDirectory (const WString& name) -> int
{
    int i;
    for (i = 0; i < _dir.size(); i++) {
        if (_dir.at(i).getFolderName() == name)
            break;
    }

    if (i == _dir.size())
        return -1;

    return this->removeDirectory(i);
}

auto FileManager::removeDirectory (int index) -> int
{
    if (Directory::removeDir(std::filesystem::path(_basePath.toStdString()) / _dir.at(index).getFolderName().toStdString()) < 0)
        return -1;

    _dir.remove(index);

    W_EMIT_0(onDirectoryListChanged);

    // TODO adjust
    W_EMIT_0(onCurrentDirectoryChanged);
    return 0;
}

auto FileManager::selectedDirectory(int index) -> bool
{
    if (_selected != index) {
        _selected = index;
        W_EMIT_0(onCurrentDirectoryChanged);
        return true;
    }

    return false;
}

auto FileManager::getCurrentDirectory() const -> const Directory&
{
    return this->_dir[_selected];
}

auto FileManager::getCurrentDirectory() -> Directory&
{
    return this->_dir[_selected];
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

auto FileManager::createDirectory(const WString &name) -> int
{
    const auto path = std::filesystem::path (_basePath.toStdString()) / name.toStdString();

    if (Directory::createDir(path))
        return -1;

    this->_dir.append(Directory(path));
    return 0;
}

// TODO: create proper file
Extention::Extention(WString e)
    : _extention(std::move(e))
{
}

auto Extention::size() const -> int
{
    return this->_extention.size();
}

auto Extention::makeWriternote() -> Extention
{
    return {"writernote"};
}
