#ifndef XMLSTRUCT_H
#define XMLSTRUCT_H

#include <iostream>
#include <cstdlib>

#include <string>
#include <fstream>
#include <streambuf>
#include <QList>
#include <QStringList>

#include "../currenttitle/currenttitle_class.h"
#include "../indice_class.h"

#include "../touch/datastruct/datastruct.h"

#include <zip.h>

class xmlstruct{
private:
    QString *path_;

    indice_class *indice = nullptr;
    currenttitle_class *currenttitle = nullptr;

    int loadbinario(struct zip *);

public:
    bool loadindice();

    bool loadfile(const char *);

    xmlstruct(QString *path_U, indice_class *indice_U, currenttitle_class *currenttitle_U);



};

#endif
