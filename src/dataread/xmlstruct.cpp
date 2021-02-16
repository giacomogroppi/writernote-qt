#include "xmlstruct.h"
#include "../self_class.h"

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>

xmlstruct::xmlstruct(QString *path_U, indice_class *indice_U, currenttitle_class *currenttitle_U)
{
    this->path_ = path_U;
    this->indice = indice_U;
    this->currenttitle = currenttitle_U;
}

