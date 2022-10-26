#pragma once

#include <zip.h>
#include "utils/common_error_definition.h"
#include "utils/WCommonScript.h"
#include "core/WZipWriter.h"
#include <QString>

class Document;

int save_audio_file(const char *posAudio,
                    const QByteArray &path);

class savefile
{
private:
    const QByteArray    *_path;
    Document            *_doc;

    int salvabinario(cbool saveImg);

public:
    savefile(const QByteArray *path, Document *currenttitle);
    ~savefile() = default;

    void setData(const QByteArray *p, Document *curr);

    int savefile_check_file(cbool saveImg);

    static int moveFileIntoZip  (const QByteArray &pathFile, WZipWriter &writer, const QByteArray &nameInZip);
};

inline savefile::savefile(const QByteArray *path, Document *currenttitle)
{
    setData(path, currenttitle);
}

inline void savefile::setData(const QByteArray *p, Document *curr)
{
    if(p){
        this->_path = p;
    }
    this->_doc = curr;
}
