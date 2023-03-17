#pragma once

#include <QByteArray>
#include <QList>
#include "File.h"

class Directory
{
private:
    QList<File> _files;
    QByteArray _path;

    QList<File> getAllFile(const QByteArray &path);
public:
    explicit Directory(const QByteArray &path);
    ~Directory();

    const QList<File>& getFiles() const;
};


