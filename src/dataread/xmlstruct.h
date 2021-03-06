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

/*
 * call then we are starting an audio
*/
int load_audio(QByteArray *array, QString &namecopybook, QString &path);

class xmlstruct{
private:
    QString *path_;

    indice_class *indice = nullptr;
    currenttitle_class *currenttitle = nullptr;

    int loadbinario(struct zip *);

#ifdef ALL_VERSION
    int load_file_2(currenttitle_class *, zip_file_t *f, zip_t *filezip);
#endif

    int load_file_3(currenttitle_class *, zip_file_t *f, zip_t *filezip);


public:
    bool loadindice();

    int loadfile(const char *);

    xmlstruct(QString *path_U, indice_class *indice_U, currenttitle_class *currenttitle_U);



};

#endif
