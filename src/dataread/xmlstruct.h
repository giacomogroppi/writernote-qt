#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <streambuf>
#include "core/WListFast.h"
#include "currenttitle/document.h"
#include "touch/dataTouch/datastruct/DataStruct.h"
#include "FileContainer/WZip.h"

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

    /* new */ int loadbinario_4(class WZip &zip, int ver_stroke);
    /* new */ int load_file_9(Document *doc, WZip &zip, cbool LoadPdf, cbool LoadImg);

    static unsigned char controllOldVersion(FileContainer *file);

    static void decode0(Document *doc, WListFast<struct point_last> &point, WListFast<double> &pos_foglio);
    static void decode1(Document *doc, WListFast<WListFast<struct point_old_ver_7>> &page);
public:
    static int load_stringa(FileReader *f, WString &stringa);
    static int load_stringa(FileReader *f, WByteArray &str);

    static int readFile(FileContainer *fileZip, WByteArray &arr,
                        cbool clear, const WString &name,
                        cbool closeZip);

    [[nodiscard]]
    const WByteArray &getPath() const;

    /* return true if we can read this file */
    static bool manageMessage(int res, const std::function<void(const WString &)>& showMessage);

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

