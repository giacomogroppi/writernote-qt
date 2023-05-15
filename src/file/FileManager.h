#pragma once

#include "utils/WCommonScript.h"
#include "Directory.h"
#include "Scheduler/WObject.h"
#include "core/WByteArray.h"
#include "core/WList.h"

class FileManager final: public WObject
{
private:
    const WByteArray _basePath;
    WList<Directory> _dir;

    static WList<Directory> getAllDir(const WByteArray &path);
public:
    explicit FileManager(WObject *parent, WByteArray basePath);
    ~FileManager() final;

    const WList<Directory> &getDirectory() const;

    int createNewDir();

    W_EMITTABLE_0(onDirectoryChange);
};
