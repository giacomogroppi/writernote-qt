/*
  Gestisce la classe fileC che gestisce il decode dei file
  e l'archiviazione dei file
*/
#include "xmlstruct.h"
#include "../self_class.h"

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QDebug>
#include <stdlib.h>
#include <assert.h>
#include <QStringList>
#include <string.h>
#define NOTFOUND -1
#include <array>
#include <cstring>
#include <iostream>
#include "../dialog_critic.h"
#include <zip.h>


using namespace std;

xmlstruct::xmlstruct(QString *path_U, indice_class *indice_U, currenttitle_class *currenttitle_U)
{
    this->path_ = path_U;
    this->indice = indice_U;
    this->currenttitle = currenttitle_U;
}

