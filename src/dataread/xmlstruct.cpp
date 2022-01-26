#include "xmlstruct.h"
#include <QString>
#include "utils/common_script.h"

xmlstruct::xmlstruct(const QString *path_U, Document *currenttitle_U)
{
    setData(path_U, currenttitle_U);
}

void xmlstruct::setData(const QString *path_U, Document *currenttitle_U)
{
    this->path_ = path_U;
    this->currenttitle = currenttitle_U;
}

zip_t *xmlstruct::openZip(const QByteArray &path, xmlstruct::openMode mode)
{
    int err;
    int flag = 0;
    const char *nameFile = path.constData();

    if(mode & openMode::readOnly)
        flag |= ZIP_RDONLY;
    if(mode & openMode::write){
        flag |= ZIP_CREATE;
    }

    return zip_open(nameFile, flag, &err);
}

zip_file_t *xmlstruct::openFile(zip_t *zip, const QByteArray &path)
{
    return zip_fopen(zip, path.constData(), 0);
}

size_t xmlstruct::get_size_file(const QByteArray &path)
{
    size_t size;
    QFile file(path);

    if(unlikely(!file.open(QIODevice::ReadOnly)))
        return 0;

    size = file.size();

    file.close();

    return size;
}

xmlstruct::xmlstruct(const QString &path_U, Document &currenttitle_U)
{
    setData(&path_U, &currenttitle_U);
}
