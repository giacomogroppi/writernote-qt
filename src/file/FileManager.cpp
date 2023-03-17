#include "FileManager.h"
#include <QDir>
#include <utility>

FileManager::FileManager(QObject *parent, QByteArray basePath)
    : QObject(parent)
    , _basePath(std::move(basePath))
    , _dir(FileManager::getAllDir(_basePath))
{
}

FileManager::~FileManager() = default;

const QList<Directory> &FileManager::getDirectory() const
{
    return this->_dir;
}

QList<Directory> FileManager::getAllDir(const QByteArray &path)
{
    QList<Directory> ret = {};
    QDir dir(path);
    QStringList directories = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const auto &folder: directories) {
        ret.append(Directory(folder.toUtf8()));
    }

    return ret;
}

