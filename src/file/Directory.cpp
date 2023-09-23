#include "Directory.h"
#include "core/String/WString.h"
#include <filesystem>

Directory::Directory(const WByteArray &path)
    : _files(Directory::getAllFile(path))
    , _path(path)
{
}

Directory::Directory(const std::filesystem::path &path)
    : _files (Directory::getAllFile(path))
    , _path(path.string().c_str())
{

}

Directory::~Directory() = default;

auto Directory::getFiles() const -> const WListFast<WFile> &
{
    return this->_files;
}

auto Directory::getAllFile(const std::filesystem::path& path) -> WListFast<WFile>
{
    WListFast<WFile> ret = {};
    std::error_code error;
    const auto iterator = std::filesystem::directory_iterator(path, error);

    if (error) {
        WDebug(true, "Warning folder not exists");
        return {};
    }

    for (const auto & entry : iterator) {
        ret.append(WFile(WByteArray(entry.path().c_str())));
    }

    return ret;
}

auto Directory::getAllFile(const WByteArray &path) -> WListFast<WFile>
{
    return Directory::getAllFile(std::filesystem::path(path.toStdString()));
}

auto Directory::allDirsInFolder() const -> WListFast<WByteArray>
{
    WListFast<WByteArray> res;

    res.reserve(_files.size());

    for (const auto& entry : std::as_const(_files)) {
        res.append(entry.getName());
    }

    return res;
}

// TODO: manage error
auto Directory::moveAllFilesTo(const WString &newPath) -> void
{
    namespace fs = std::filesystem;
    std::error_code error;

    // before move files we need to close everything
    this->_files.clear();

    fs::rename(fs::path(_path.toStdString()), fs::path(newPath.toStdString()), error);

    if (error) {
        W_ASSERT_TEXT(0, "move file error...");
    }

    _path = newPath.toUtf8();

    _files = Directory::getAllFile(_path);
}

auto Directory::getFolderName() const -> WString
{
    W_ASSERT(_path[_path.size() - 1] != slash::__slash());

    return this->_path.split(slash::__slash()).last();
}

auto Directory::removeDir(const std::filesystem::path &path) -> int
{
    if (std::filesystem::remove_all(path) == true)
        return 0;
    return -1;
}

int Directory::removeDir(const WByteArray &path)
{
    const auto res = std::filesystem::remove_all(path.constData());

    if (res)
        return 0;
    return -1;
}

auto Directory::createDir(const std::filesystem::path &path) -> int
{
    if (std::filesystem::create_directories(path))
        return 0;
    return -1;
}

auto Directory::createDir(const WByteArray &path) -> int
{
    if (std::filesystem::create_directories(path.constData()))
        return 0;
    return -1;
}

bool Directory::exists(const WByteArray &path)
{
    return std::filesystem::is_directory(path.toStdString());
}

auto Directory::removeFile(const WString& name) -> int
{
    std::error_code error;
    std::filesystem::path path (WString(_path).addSlashIfNecessary().toStdString());

    std::filesystem::remove(path / std::filesystem::path(name.toStdString()), error);
    return error.operator bool() ? -1: 0;
}

auto Directory::addFiles(const std::filesystem::path &position) -> int
{
    if (WFile::exists(position))
        return -1;

    this->_files.append(WFile(position));

    return 0;
}
