#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <streambuf>
#include "core/WListFast.h"
#include "currenttitle/document.h"
#include "touch/dataTouch/datastruct/DataStruct.h"
#include "core/WZip.h"

#define LOAD_MULTIPLESTRING(x, y, z) if(xmlstruct::load_multiplestring(x,y,z) == ERROR) goto free_;
#define LOAD_MULTIPLESTRING_RETURN(x, y, z) if(xmlstruct::load_multiplestring(x,y,z) == ERROR) return ERROR;
#define LOAD_STRINGA(x, y) if(xmlstruct::load_stringa(x,y) == ERROR) goto free_;
#define LOAD_STRINGA_RETURN(x, y) if(xmlstruct::load_stringa(x, y) == ERROR)return ERROR;

/*
 * call then we are starting an audio
*/
int load_audio(WByteArray &array, const WString &path);


class xmlstruct{
private:
    const WByteArray *_path;
    static constexpr size_t get_offset_start() { return sizeof(int); };
    Document *_doc = nullptr;

    /* old */ int load_file_2(WZipReaderSingle &reader);
    /* old */ int load_file_3(WZipReaderSingle &reader);
    /* old */ int load_file_4(WZipReaderSingle &reader);
    /* old */ int load_file_5(WZipReaderSingle &reader, cbool LoadPdf, cbool LoadImg);
    /* old */ int load_file_6(WZipReaderSingle &reader, cbool LoadPdf, cbool LoadImg);
    /* old */ int load_file_7(WZipReaderSingle &reader, cbool LoadPdf, cbool LoadImg);
    /* old */ int load_file_8(WZipReaderSingle &reader, cbool LoadPdf, cbool LoadImg);

    /* __old */ int loadbinario_0(class WZip &zip);
    /* __old */ int loadbinario_1(class WZip &zip);
    /* __old */ int loadbinario_2(class WZip &zip);
    /* __old */ int loadbinario_3(class WZip &zip, int ver_stroke);

    /* new */ int loadbinario_4(class WZip &zip, int ver_stroke);
    /* new */ int load_file_9(Document *doc, WZip &zip, cbool LoadPdf, cbool LoadImg);

    static int load_multiplestring(FileReader *f, WListFast<WString> &lista, WListFast<int> &data);
    static unsigned char controllOldVersion(FileContainer *file);

    static void decode0(Document *doc, WListFast<struct point_last> &point, WListFast<double> &pos_foglio);
    static void decode1(Document *doc, WListFast<WListFast<struct point_old_ver_7>> &page);
public:
    static size_t sizeFile(FileContainer *filezip, const WString &namefile);
    static size_t sizeFile(FileContainer *filezip, const char *namefile);

    static int load_stringa(FileReader *f, WString &stringa);
    static int load_stringa(FileReader *f, WByteArray &str);

    static int readFile(FileContainer *fileZip, WByteArray &arr,
                        cbool clear, const WString &name,
                        cbool closeZip);

    const WByteArray &getPath() const;

    /* return true if we can read this file */
    static bool manageMessage(const int res, std::function<void(const WString &)> showMessage);

    int xmlstruct_read_file_old(int ver, WZip &zip, cbool LoadPdf, cbool LoadImg);

    int loadfile(cbool LoadPdf, cbool LoadImg);

    xmlstruct(const WByteArray *path_U, Document *currenttitle_U);
    xmlstruct(const WByteArray &path_U, Document &currenttitle_U);

    void setData(const WByteArray *path_U, Document *currenttitle_U);

    static size_t get_size_file(const WByteArray &path);
    static bool closeZip(FileContainer *fileZip);
};

inline const WByteArray &xmlstruct::getPath() const
{
    return *_path;
}

force_inline size_t xmlstruct::sizeFile(FileContainer *filezip, const WString &namefile)
{
    return xmlstruct::sizeFile(filezip, namefile.toUtf8().constData());
}

force_inline bool xmlstruct::closeZip(FileContainer *fileZip)
{
    return fileZip->close();
}

