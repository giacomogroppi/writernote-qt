#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <zip.h>
#include "../utils/common_error_definition.h"
#include <QString>
#define ERROR_PRIVATE -1
#define OK_PRIVATE 0
class currenttitle_class;
class indice_class;

#define WRITE_ON_SIZE(x,y,z) if(zip_source_write(x,y,z)==-1)goto error;

int save_audio_file(const char *posAudio,
                    const QString &namecopybook,
                    const QString &path);

class savefile
{
private:
    const QString *path;
    currenttitle_class *currenttitle;

    int salvabinario(zip_t *);

    int savefile_check_2(zip_source_t *file, currenttitle_class *currenttitle, zip_t *filezip);

public:
    static int saveArrayIntoFile(const QString &from, const QString& name_coby,
                                 const QString path, zip_t *filezip, const QString &suffix);

    void setting_data(currenttitle_class *m_current){currenttitle = m_current;}

    savefile(const QString &path, currenttitle_class *current){
        setData(&path, current);
    }

    savefile(const QString *path, currenttitle_class *currenttitle){
        setData(path, currenttitle);
    }
    savefile(const QString &path, currenttitle_class &currenttitle){
        setData(&path, &currenttitle);
    }

    void setData(const QString *p, currenttitle_class *curr){
        if(p){
            this->path = p;
        }
        this->currenttitle = curr;
    }

    int savefile_check_indice(indice_class *);
    int savefile_check_file();

};

#endif // SAVEFILE_H
