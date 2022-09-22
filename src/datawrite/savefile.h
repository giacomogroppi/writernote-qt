#pragma once

#include <zip.h>
#include "utils/common_error_definition.h"
#include "utils/common_script.h"
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
    static void removeFile(zip_t *file, const QString &name);
    /*
     * if filezip is NULL it will open the file
    */
    static int moveFileIntoZip  (const QByteArray &pathFile, WZipWriter &writer, const QByteArray &nameInZip);

    static uchar save_string(zip_source_t *file, const char *str);

    savefile(const QString *path, Document *currenttitle){ setData(path, currenttitle); }

    void setData(const QString *p, Document *curr);

    int savefile_check_file(cbool saveImg);
};

force_inline void savefile::setData(const QString *p, Document *curr)
{
    if(p){
        this->_path = p;
    }
    this->_doc = curr;
}
