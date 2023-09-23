#pragma once

#include "utils/WCommonScript.h"
#include "Directory.h"
#include "Scheduler/WObject.h"
#include "core/ByteArray/WByteArray.h"
#include "core/WList.h"

class FileManager final:
        public WObject
{
private:
    WByteArray _basePath;
    WListFast<Directory> _dir;
    unsigned short _selected;

    static WListFast<Directory> getAllDir(const WByteArray &path);
public:
    explicit FileManager(WObject *parent, WByteArray basePath, bool createDir);
    ~FileManager() final;

    auto getDirectory() const -> const WListFast<Directory> &;
    auto getCurrentPath() const -> WString;

    auto getCurrentFiles() const -> const WListFast<WFile>&;

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
    auto moveTo(const WString& newPath) -> void;

    auto createDirectory (const WString& name) -> int;
    auto createFile (const WString& name) -> int;

    W_EMITTABLE_0(onDirectoryListChanged);
    W_EMITTABLE_0(onCurrentDirectoryChanged);
    W_EMITTABLE_0(onListFilesChanged);
};
