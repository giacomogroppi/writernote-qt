#pragma once

#include "utils/WCommonScript.h"
#include "Directory.h"
#include "Scheduler/WObject.h"
#include "core/ByteArray/WByteArray.h"
#include "core/WList.h"

// TODO: create proper file
class Extention {
private:
    WString _extention;

    Extention (WString e);
public:

    static auto makeWriternote() -> Extention;

    [[nodiscard]]
    auto size() const -> int;

    operator std::filesystem::path () const { return std::filesystem::path (_extention.toStdString()); }
    operator WString () const { return _extention; }
};

class FileManager final:
        public WObject
{
private:
    WPath _basePath;
    WListFast<Directory> _dir;
    unsigned short _selected;

    static WListFast<Directory> getAllDir(const WPath &path);
public:
    explicit FileManager(WObject *parent, WPath basePath, bool createDir);
    ~FileManager() final = default;

    [[nodiscard]]
    auto getDirectory() const -> const WListFast<Directory> &;

    [[nodiscard]]
    auto getCurrentPath() const -> WPath;

    [[nodiscard]]
    auto getCurrentFiles() const -> const WListFast<WFile>&;

    [[nodiscard]]
    auto getCurrentDirectory() const -> const Directory&;
    auto getCurrentDirectory() -> Directory&;

    auto removeDirectory (const WString& name) -> int;
    auto removeDirectory (int index) -> int;

    /**
     * \return true if the current directory has changed
     * */
    auto selectedDirectory(int index) -> bool;

    /**
     * \paragraph This method move all the files from the current directory to the new one
     * \param newPath New path to move all the file in the current directory
     * */
    auto moveTo(const WPath& newPath) -> void;

    auto createDirectory (const WString& name) -> int;

    template <class T>
            requires (std::is_class<T>::value)
    auto createFile (const WString& name, const T& file, const Extention& extension) -> int;

    template <class T>
            requires (std::is_class<T>::value)
    [[nodiscard]]
    auto openFile (const WString& name, const Extention& extension) -> WPair<int, T>;

    W_EMITTABLE_0(onDirectoryListChanged);
    W_EMITTABLE_0(onCurrentDirectoryChanged);
    W_EMITTABLE_0(onListFilesChanged);
};

template <class T>
    requires (std::is_class<T>::value)
inline auto FileManager::createFile(const WString& name, const T &file, const Extention &extension) -> int
{
    auto& dir = _dir[_selected];
    const WString nameWithExtension = (name.mid(name.size() - extension.size()) == ('.' + extension)) ?
                                       name : WString(name + '.' + extension);
    const auto path = _basePath
                        / dir.getFolderName()
                        / nameWithExtension;

    const auto result = _dir[_selected].addFiles(path, file);

    W_EMIT_0(onListFilesChanged);

    return result;
}

template <class T>
    requires (std::is_class<T>::value)
inline auto FileManager::openFile(const WString &name, const Extention &extension) -> WPair<int, T>
{
    const auto path = _basePath
            / _dir[_selected].getFolderName()
            / (name + '.' + extension);

    WFile file (path, WFile::WFileReadOnly);

    const auto result = VersionFileController::load (file);

    if (result.first < 0)
        return {-1, {}};

    const auto data = T::load (result.second, file);

    if (data.first < 0)
        return {-1, {}};

    return {0, {}};
}

inline auto FileManager::getDirectory() const -> const WListFast<Directory> &
{
    return this->_dir;
}
