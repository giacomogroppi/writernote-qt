#pragma once

#include "File.h"
#include "core/WList.h"
#include "core/WByteArray.h"

class Directory
{
private:
    WList<File> _files;
    WByteArray _path;

    static WList<File> getAllFile(const WByteArray &path);
public:
    explicit Directory(const WByteArray &path);
    ~Directory();

    const WList<File>& getFiles() const;

    /**
     * requires
     *  file in position don't exists
    */
    bool addFiles(const WByteArray &position);

    WList<WByteArray> allDirsInFolder() const;
};


