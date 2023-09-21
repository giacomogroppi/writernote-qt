#include "Directory.h"
#include "core/String/WString.h"
#include <filesystem>

Directory::Directory(const WByteArray &path)
    : _files(Directory::getAllFile(path))
    , _path(path)
{
}

Directory::~Directory() = default;

auto Directory::getFiles() const -> const WList<WFile> &
{
    return this->_files;
}

auto Directory::addFiles(const WByteArray &position) -> bool
{
    this->_files.append(WFile(position));

    return true;
}

auto Directory::getAllFile(const WByteArray &path) -> WList<WFile>
{
    WList<WFile> ret = {};
    std::error_code error;
    const auto iterator = std::filesystem::directory_iterator(path.toStdString(), error);

    if (error) {
        WDebug(true, "Warning folder not exists");
        return {};
    }

    for (const auto & entry : iterator) {
        ret.append(WFile(WByteArray(entry.path().c_str())));
    }

    return ret;
}

auto Directory::allDirsInFolder() const -> WList<WByteArray>
{
    WList<WByteArray> res {};

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
