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


class xmlstruct
{
private:
    const WPath *_path;
    static constexpr size_t get_offset_start() { return sizeof(int); };
    Document *_doc = nullptr;

    constexpr static const auto nameFile = "FileTouch";
    constexpr static const auto nameAudio = "FileAudio";

public:
    static int readFile(FileContainer *fileZip, WByteArray &arr,
                        cbool clear, const WString &name,
                        cbool closeZip);

    [[nodiscard]]
    auto getPath() const -> const WPath &;

    /* return true if we can load this file */
    static bool manageMessage(int res, const Fn<void(const WString &)>& showMessage);

    int xmlstruct_read_file_old(int ver, WZip &zip, cbool LoadPdf, cbool LoadImg);

    int loadfile(cbool LoadPdf, cbool LoadImg);

    xmlstruct(const WPath *path_U, Document *currenttitle_U);
    xmlstruct(const WPath &path_U, Document &currenttitle_U);

    void setData(const WPath *path_U, Document *currenttitle_U);

    static auto get_size_file(const WPath &path) -> size_t;
    static auto closeZip(FileContainer *fileZip) -> bool;
};

inline auto xmlstruct::getPath() const -> const WPath &
{
    return *_path;
}

