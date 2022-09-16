#pragma once

#include <zip.h>
#include "utils/common_error_definition.h"
#include "utils/common_script.h"
#include <QString>

class Document;

int save_audio_file(const char *posAudio,
                    const QString &path);

class savefile
{
private:
    const QString *path;
    Document *currenttitle;

    int salvabinario(zip_t *zz, cbool saveImg);

    int savefile_check_2(zip_source_t *file, Document *currenttitle, zip_t *filezip);

public:
    static void removeFile(zip_t *file, const QString &name);
    /*
     * if filezip is NULL it will open the file
    */
    static int moveFileIntoZip  (const QString &from,   const QString &path, zip_t *filezip, const QString &name, const bool closeZip);
    static int saveArrayIntoFile(const QByteArray &arr, const QString &path, zip_t *filezip, const QString &name, bool closeZip);

    static uchar save_string(zip_source_t *file, const char *str);

    static bool addFile(zip_t *fileZip, cchar *fileName, zip_source_t *file);
    static bool commitChange(zip_source_t *file);

    force_inline void setting_data(Document *m_current){currenttitle = m_current;}

    force_inline savefile(const QString &path, Document *current){ setData(&path, current); }

    force_inline savefile(const QString *path, Document *currenttitle){ setData(path, currenttitle); }
    force_inline savefile(const QString &path, Document &currenttitle){ setData(&path, &currenttitle); }

    void setData(const QString *p, Document *curr);

    int savefile_check_file(cbool saveImg);

    static uchar saveArrIntoFile(const QByteArray &arr, const QString &path);
};

force_inline void savefile::setData(const QString *p, Document *curr)
{
    if(p){
        this->path = p;
    }
    this->currenttitle = curr;
}

// return true when success
force_inline bool savefile::commitChange(zip_source_t *file)
{
    /*
     * Upon successful completion 0 is returned. Otherwise, -1 is returned
     * and the error information in source is set to indicate the error.
    */
    if(unlikely(zip_source_commit_write(file) == -1))
        return false;
    return true;
}

// return true when success
force_inline bool savefile::addFile(zip_t *fileZip, const char *fileName, zip_source_t *file)
{
    /*
     * Upon successful completion, zip_file_add() returns the index of
     * the new file in the archive, and zip_file_replace() returns 0.
     * Otherwise, -1 is returned and the error code in archive is set
     * to indicate the error.
    */
    if(unlikely(zip_file_add(fileZip, fileName, file, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8) == -1))
        return false;
    return true;
}

