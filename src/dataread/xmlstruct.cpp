#include "xmlstruct.h"
#include <QString>

xmlstruct::xmlstruct(const QString *path_U, Document *currenttitle_U)
{
    setData(path_U, currenttitle_U);
}

void xmlstruct::setData(const QString *path_U, Document *currenttitle_U)
{
    this->path_ = path_U;
    this->currenttitle = currenttitle_U;
}

xmlstruct::xmlstruct(const QString &path_U, Document &currenttitle_U){
    setData(&path_U, &currenttitle_U);
}
