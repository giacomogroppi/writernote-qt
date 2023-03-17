#pragma once

#include <QObject>
#include "utils/WCommonScript.h"
#include <QList>
#include "Directory.h"

class FileManager final: public QObject
{
    Q_OBJECT
private:
    const QByteArray _basePath;
    QList<Directory> _dir;

    static QList<Directory> getAllDir(const QByteArray &path);
public:
    explicit FileManager(QObject *parent, QByteArray basePath);
    ~FileManager() final;

    const QList<Directory> &getDirectory() const;

    int createNewDir();
signals:
    void onDirectoryChange();
};
