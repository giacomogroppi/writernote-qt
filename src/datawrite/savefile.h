#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <zip.h>
#include "../utils/common_error_definition.h"
#include <QString>
#define ERROR_PRIVATE -1
#define OK_PRIVATE 0
class Document;

#define WRITE_ON_SIZE(x,y,z) if(zip_source_write(x,y,z)==-1)goto error;

int save_audio_file(const char *posAudio,
                    const QString &path);

class savefile
{
private:
    const QString *path;
    Document *currenttitle;

    int salvabinario(zip_t *);

    int savefile_check_2(zip_source_t *file, Document *currenttitle, zip_t *filezip);

public:
    /*
     * if filezip is NULL it will open the file
    */
    static int saveArrayIntoFile(const QString &from,
                                 const QString &path, zip_t *filezip, const QString &name, const bool closeZip);
    static int saveArrayIntoFile(const QByteArray &arr,
                                 const QString &path, zip_t *filezip, const QString &name, const bool closeZip);

    static uchar save_string(zip_source_t *file, const char *str);

    void setting_data(Document *m_current){currenttitle = m_current;}

    savefile(const QString &path, Document *current){
        setData(&path, current);
    }

    savefile(const QString *path, Document *currenttitle){
        setData(path, currenttitle);
    }
    savefile(const QString &path, Document &currenttitle){
        setData(&path, &currenttitle);
    }

    void setData(const QString *p, Document *curr){
        if(p){
            this->path = p;
        }
        this->currenttitle = curr;
    }

    int savefile_check_file();

    static uchar saveArrIntoFile(const QByteArray &arr, const QString &path);
};

#endif // SAVEFILE_H
