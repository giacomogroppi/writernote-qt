#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <streambuf>
#include <QList>
#include <QStringList>
#include "currenttitle/document.h"
#include "touch/datastruct/datastruct.h"
#include <zip.h>
#include "core/WZip.h"

#define LOAD_MULTIPLESTRING(x, y, z) if(xmlstruct::load_multiplestring(x,y,z) == ERROR) goto free_;
#define LOAD_MULTIPLESTRING_RETURN(x, y, z) if(xmlstruct::load_multiplestring(x,y,z) == ERROR) return ERROR;
#define LOAD_STRINGA(x, y) if(xmlstruct::load_stringa(x,y) == ERROR) goto free_;
#define LOAD_STRINGA_RETURN(x, y) if(xmlstruct::load_stringa(x, y) == ERROR)return ERROR;

#define __new
#define __old

/*
 * call then we are starting an audio
*/
int load_audio(QByteArray &array, const QString &path);

force_inline zip_file_t *zip_fopen(zip_t *zip, const QString &path, zip_flags_t flag)
{
    return zip_fopen(zip, path.toUtf8().constData(), flag);
};

force_inline zip_t *zip_open(const QString &path, int check, int *ok)
{
    return zip_open(path.toUtf8().constData(), check, ok);
}

class xmlstruct{
private:
    const QByteArray *_path;
    static constexpr size_t get_offset_start() { return sizeof(int); };
    Document *_doc = nullptr;

#ifdef ALL_VERSION
    __old int load_file_2(WZipReaderSingle &reader);
    __old int load_file_3(WZipReaderSingle &reader);
    __old int load_file_4(WZipReaderSingle &reader);
    __old int load_file_5(WZipReaderSingle &reader, const bool LoadPdf, const bool LoadImg);
    __old int load_file_6(WZipReaderSingle &reader, const bool LoadPdf, const bool LoadImg);
    __old int load_file_7(WZipReaderSingle &reader, const bool LoadPdf, const bool LoadImg);
    __old int load_file_8(WZipReaderSingle &reader, const bool LoadPdf, const bool LoadImg);

    __old int loadbinario_0(class WZip &zip);
    __old int loadbinario_1(class WZip &zip);
    __old int loadbinario_2(class WZip &zip);
    __old int loadbinario_3(class WZip &zip, int ver_stroke);

#endif // ALL_VERSION

    __new int loadbinario_4(class WZip &zip, int ver_stroke);
    __new int load_file_9(Document *doc, WZip &zip, const bool LoadPdf, const bool LoadImg);

    static int load_multiplestring(zip_file_t *f, QList<QString> &lista, QList<int> &data);
    static uchar controllOldVersion(zip_t *file);

    static void decode0(Document *doc, QList<struct point_last> &point, QList<double> &pos_foglio);
    static void decode1(Document *doc, QList<QList<struct point_old_ver_7>> &page);
public:
    static size_t sizeFile(zip_t *filezip, const QString &namefile);
    static size_t sizeFile(zip_t *filezip, const char *namefile);

    static int load_stringa(zip_file_t *f, QString &stringa);
    static int load_stringa(zip_file_t *f, QByteArray &str);

    static int readFile(zip_t *fileZip, QByteArray &arr,
                        const bool clear, const QString &name,
                        const bool closeZip);

    const QByteArray &getPath() const;

    /* return true if we can read this file */
    static bool manageMessage(const int res);

#ifdef ALL_VERSION
    int xmlstruct_read_file_old(int ver, WZip &zip, cbool LoadPdf, cbool LoadImg);
#endif
    int loadfile(const bool LoadPdf, const bool LoadImg);

    xmlstruct(const QByteArray *path_U, Document *currenttitle_U);
    xmlstruct(const QByteArray &path_U, Document &currenttitle_U);

    void setData(const QByteArray *path_U, Document *currenttitle_U);

    static size_t get_size_file(const QByteArray &path);
    static bool closeZip(zip_t *fileZip);
};

inline const QByteArray &xmlstruct::getPath() const
{
    return *_path;
}

force_inline size_t xmlstruct::sizeFile(zip_t *filezip, const QString &namefile)
{
    return xmlstruct::sizeFile(filezip, namefile.toUtf8().constData());
}

force_inline bool xmlstruct::closeZip(zip_t *fileZip)
{
    if(unlikely(zip_close(fileZip)) == -1){
        return false;
    }
    return true;
}

