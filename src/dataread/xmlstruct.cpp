#include "xmlstruct.h"

#include <QString>

xmlstruct::xmlstruct(const QString *path_U, indice_class *indice_U, currenttitle_class *currenttitle_U)
{
    setData(path_U, indice_U, currenttitle_U);
}

void xmlstruct::setData(const QString *path_U, indice_class *indice_U, currenttitle_class *currenttitle_U)
{
    this->path_ = path_U;
    this->indice = indice_U;
    this->currenttitle = currenttitle_U;
}

xmlstruct::xmlstruct(const QString &path_U, indice_class &indice_U, currenttitle_class &currenttitle_U){
    setData(&path_U, &indice_U, &currenttitle_U);
}
