#include "FileManager.h"
#include "core/WDir.h"
#include <utility>
#include <filesystem>

static const WListFast<WFile> emptyFiles;

FileManager::FileManager(WObject *parent, WPath basePath, bool createDir)
    : WObject(parent)
    , _basePath(std::move(basePath))
    , _dir(FileManager::getAllDir(_basePath))
    , _selected(0)
{
    if (createDir and _dir.isEmpty()) {
        // maybe basePath don't exist
        (void)Directory::createDir(_basePath);
    }

    if (_dir.isEmpty() and createDir) {
        (void)Directory::createDir(_basePath / "Not classified");
        _dir = FileManager::getAllDir(_basePath);
    }
}

auto FileManager::getCurrentFiles() const -> const WListFast<WFile>&
{
    if (_dir.isEmpty())
        return emptyFiles;
    return _dir.at(_selected).getFiles();
}

auto FileManager::getAllDir(const WPath &path) -> WListFast<Directory>
{
    std::error_code error;
    WListFast<Directory> ret = {};
    auto entry = std::filesystem::directory_iterator(path, error);

    if (error.value() != 0) {
        return {};
    }

    for (const auto& ref: std::as_const(entry)) {
        ret.append(Directory(WPath(ref.path())));
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
    if (Directory::removeDir(_basePath / _dir.at(index).getFolderName()) < 0)
        return -1;

    _dir.removeAt(index);

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
    return this->_dir.at(_selected);
}

auto FileManager::getCurrentDirectory() -> Directory&
{
    return this->_dir[_selected];
}

auto FileManager::getCurrentPath() const -> WPath
{
    return _basePath;
}

auto FileManager::moveTo(const WPath& newPath) -> void
{
    Directory dir (this->_basePath);
    dir.moveAllFilesTo(newPath);

    this->_basePath = newPath;

    _dir = FileManager::getAllDir(_basePath);
}

auto FileManager::createDirectory(const WString &name) -> int
{
    const auto path = _basePath / name;

    if (Directory::createDir(path))
        return -1;

    this->_dir.append(Directory(path));
    return 0;
}

// TODO: create proper file
Extension::Extension(WString e)
    : _extension(std::move(e))
{
}

auto Extension::size() const -> int
{
    return this->_extension.size();
}

auto Extension::makeWriternote() -> Extension
{
    return {"writernote"};
}

auto Extension::makeTxt() -> Extension
{
    return {"txt"};
}
