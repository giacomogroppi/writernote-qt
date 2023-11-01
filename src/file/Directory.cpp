#include "Directory.h"
#include "core/String/WString.h"
#include <filesystem>

Directory::Directory(const WPath &path)
    : _files (Directory::getAllFiles(path))
    , _path(path)
{

}

Directory::~Directory() = default;

auto Directory::getFiles() const -> const WListFast<WFile> &
{
    return this->_files;
}

auto Directory::getAllFiles(const WPath& path) -> WListFast<WFile>
{
    WListFast<WFile> ret = {};
    std::error_code error;
    const auto iterator = std::filesystem::directory_iterator(path, error);

    if (error) {
        WWarning("Warning folder not exists");
        return {};
    }

    for (const auto & entry : iterator) {
        WDebug(debug, "Name of the file is:" << entry.path().string());
        ret.append(WFile(entry.path()));
    }

    return ret;
}

auto Directory::allDirsInFolder() const -> WListFast<WByteArray>
{
    WListFast<WByteArray> res;

    res.reserve(_files.size());

    for (const auto& entry : std::as_const(_files)) {
        WDebug(debug, entry.getName());
        res.append(entry.getName());
    }

    return res;
}

// TODO: manage error
auto Directory::moveAllFilesTo(const WPath &newPath) -> void
{
    namespace fs = std::filesystem;
    std::error_code error;

    if (newPath == _path)
        return;

    // before move files, we need to close everything
    this->_files.clear();

    fs::rename(_path, newPath, error);

    if (error) {
        W_ASSERT_TEXT(0, "move file error... from" << _path << "to" << newPath);
    }

    _path = newPath;

    _files = Directory::getAllFiles(_path);
}

auto Directory::getFolderName() const -> WString
{
    return this->_path.getNameWithExtension();
}

auto Directory::removeDir(const WPath& path) -> int
{
    if (std::filesystem::remove_all(path) == true)
        return 0;
    return -1;
}

auto Directory::createDir(const WPath& path) -> int
{
    if (std::filesystem::create_directories(path))
        return 0;
    return -1;
}

auto Directory::removeFile(const WString& name) -> int
{
    std::error_code error;

    std::filesystem::remove(_path / std::filesystem::path(name.toStdString()), error);

    if (not error.operator bool()) {
        for (int i = 0; i < _files.size(); i++)
            if (_files[i].getName() == name) {
                _files.removeAt(i);
                break;
            }
    }

    return error.operator bool() ? -1: 0;
}
