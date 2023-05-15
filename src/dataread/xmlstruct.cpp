#include "xmlstruct.h"
#include "core/WString.h"
#include "utils/WCommonScript.h"
#include <QFile>

xmlstruct::xmlstruct(const QByteArray *path_U, Document *currenttitle_U)
{
    setData(path_U, currenttitle_U);
}

void xmlstruct::setData(const QByteArray *path_U, Document *currenttitle_U)
{
    this->_path = path_U;
    this->_doc = currenttitle_U;
}

size_t xmlstruct::get_size_file(const QByteArray &path)
{
    size_t size;
    QFile file(path);

    if(un(!file.open(QIODevice::ReadOnly)))
        return 0;

    size = file.size();

    file.close();

    return size;
}

xmlstruct::xmlstruct(const QByteArray &path_U, Document &currenttitle_U)
{
    setData(&path_U, &currenttitle_U);
}
