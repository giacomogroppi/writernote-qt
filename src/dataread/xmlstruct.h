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
int load_audio(QByteArray &array, const QString &namecopybook, const QString &path);
inline zip_file_t *zip_fopen(zip_t *zip, const QString &path, zip_flags_t flag){
    return zip_fopen(zip, path.toUtf8().constData(), flag);
};

class xmlstruct{
private:
    const QString *path_;

    indice_class *indice = nullptr;
    currenttitle_class *currenttitle = nullptr;

#ifdef ALL_VERSION
    int load_file_2(currenttitle_class *, zip_file_t *f, zip_t *filezip);
    int load_file_3(currenttitle_class *, zip_file_t *f, zip_t *filezip);
    int loadbinario_0(struct zip *);
#endif
    int load_file_4(currenttitle_class *, zip_file_t *f, zip_t *filezip);
    int loadbinario_1(struct zip *);

public:
    static size_t sizeFile(zip_t *filezip, const QString &namefile){
        return xmlstruct::sizeFile(filezip, namefile.toUtf8().constData());
    }
    static size_t sizeFile(zip_t *filezip, const char *namefile);
    static int load_stringa(zip_file_t *f, QString &stringa);
    static int readFile(zip_t *fileZip, QByteArray &arr,
                        const bool clear, const QString &name,
                        const bool closeZip);

    QString getPath(){
        return *path_;
    };

    bool loadindice();

    int loadfile(const char *);
    int loadfile(const QString &path){
        return loadfile(path.toUtf8().constData());
    }

    xmlstruct(const QString *path_U, indice_class *indice_U, currenttitle_class *currenttitle_U);
    xmlstruct(const QString &path_U, indice_class &indice_U, currenttitle_class &currenttitle_U);

    void setData(const QString *path_U, indice_class *indice_U, currenttitle_class *currenttitle_U);

};

#endif
