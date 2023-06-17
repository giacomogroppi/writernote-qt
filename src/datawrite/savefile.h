#pragma once

#include "utils/common_error_definition.h"
#include "utils/WCommonScript.h"
#include "FileContainer/WZipWriter.h"
#include "core/ByteArray/WByteArray.h"

class Document;

int save_audio_file(const char *posAudio,
                    const WByteArray &path);

class savefile
{
private:
    const WByteArray    *_path;
    Document            *_doc;

    int salvabinario(cbool saveImg);

public:
    savefile(const WByteArray *path, Document *currenttitle);
    ~savefile() = default;

    void setData(const WByteArray *p, Document *curr);

    int savefile_check_file(cbool saveImg);

    static int moveFileIntoZip  (const WByteArray &pathFile, WZipWriter &writer, const WByteArray &nameInZip);
};

inline savefile::savefile(const WByteArray *path, Document *currenttitle)
{
    setData(path, currenttitle);
}

inline void savefile::setData(const WByteArray *p, Document *curr)
{
    if(p){
        this->_path = p;
    }
    this->_doc = curr;
}
