#pragma once

#include "File.h"
#include "core/WList.h"
#include "core/WByteArray.h"
#include "core/WFile.h"

class Directory
{
private:
    WList<WFile> _files;
    WByteArray _path;

    static WList<WFile> getAllFile(const WByteArray &path);
public:
    explicit Directory(const WByteArray &path);
    ~Directory();

    const WList<WFile>& getFiles() const;

    /**
     * requires
     *  file in position don't exists
    */
    bool addFiles(const WByteArray &position);

    WList<WByteArray> allDirsInFolder() const;


    bool operator ==(const Directory &other) const;
    bool operator !=(const Directory &other) const;
};

inline bool Directory::operator ==(const Directory &other) const
{
    return _path == other._path;
}

inline bool Directory::operator !=(const Directory &other) const
{
    return !(*this == other);
}

