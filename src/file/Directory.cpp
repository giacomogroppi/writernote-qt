#include "Directory.h"
#include <QDir>

Directory::Directory(const QByteArray &path)
    : _files(Directory::getAllFile(path))
    , _path(path)
{
}

Directory::~Directory() = default;

const QList<File> &Directory::getFiles() const
{
    return this->_files;
}

bool Directory::addFiles(const QByteArray &position)
{
    if (!File::createFile(position)) {
        return false;
    }

    this->_files.append(File(position));

    return true;
}

QList<File> Directory::getAllFile(const QByteArray &path)
{
    QList<File> ret = {};
    QDir dir(path);
    const auto &files = dir.entryList(QDir::Files);

    for (const auto &p : files) {
        ret.append(File(p.toUtf8()));
    }

    return ret;
}

