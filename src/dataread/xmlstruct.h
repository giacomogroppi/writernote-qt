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

/*
 * call then we are starting an audio
*/
int load_audio(WByteArray &array, const WString &path);


class xmlstruct{
private:
    const WByteArray *_path;
    static constexpr size_t get_offset_start() { return sizeof(int); };
    Document *_doc = nullptr;

    constexpr static const auto nameFile = "FileTouch";
    constexpr static const auto nameAudio = "FileAudio";

public:
    static int load_stringa(FileReader *f, WString &stringa);
    static int load_stringa(FileReader *f, WByteArray &str);

    static int readFile(FileContainer *fileZip, WByteArray &arr,
                        cbool clear, const WString &name,
                        cbool closeZip);

    [[nodiscard]]
    const WByteArray &getPath() const;

    /* return true if we can load this file */
    static bool manageMessage(int res, const Fn<void(const WString &)>& showMessage);

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

