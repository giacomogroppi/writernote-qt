#ifndef XMLSTRUCT_H
#define XMLSTRUCT_H

#include <iostream>
#include <cstdlib>

#include <string>
#include <fstream>
#include <streambuf>
#include <QList>
#include <QStringList>

#include "../currenttitle/document.h"
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
inline zip_t *zip_open(const QString &path, int check, int *ok){
    return zip_open(path.toUtf8().constData(), check, ok);
}

class xmlstruct{
private:
    const QString *path_;

    indice_class *indice = nullptr;
    Document *currenttitle = nullptr;

#ifdef ALL_VERSION
    int load_file_2(Document *, zip_file_t *f, zip_t *filezip);
    int load_file_3(Document *, zip_file_t *f, zip_t *filezip);
    int loadbinario_0(struct zip *);
    int load_file_4(Document *, zip_file_t *f, zip_t *filezip);
#endif
    int load_file_5(Document *doc, zip_file_t *f, zip_t *filezip);
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

    int loadfile(const char *path);
    int loadfile(const QString &path){
        return loadfile(path.toUtf8().constData());
    }

    xmlstruct(const QString *path_U, indice_class *indice_U, Document *currenttitle_U);
    xmlstruct(const QString &path_U, indice_class &indice_U, Document &currenttitle_U);

    void setData(const QString *path_U, indice_class *indice_U, Document *currenttitle_U);

};

#endif
