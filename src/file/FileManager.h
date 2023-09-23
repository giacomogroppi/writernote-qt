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

    static WListFast<Directory> getAllDir(const WByteArray &path);
public:
    explicit FileManager(WObject *parent, WByteArray basePath, bool createDir);
    ~FileManager() final;

    auto getDirectory() const -> const WListFast<Directory> &;
    auto getCurrentPath() const -> WString;

    /**
     * \paragraph This method move all the files from the current directory to the new one
     * \param newPath New path to move all the file in the current directory
     * */
    auto moveTo(const WString& newPath) -> void;

    int createNewDir();

    W_EMITTABLE_0(onDirectoryChange);
};
