#include "../savefile.h"
#include "../source_read_ext.h"
#include "../../utils/common_error_definition.h"
#include "../../images/save_images.h"

#define SAVE_IMAGE(x, y) if(save_image(x, y) != OK) goto delete_;
#define SAVE_STRINGA(x, y) if(save_string(x, y) != OK) goto delete_;

static int save_string(zip_source_t *, const char *);

static void setCurrentVersion(currenttitle_class *data);

#define ERROR_PRIVATE -1
#define OK_PRIVATE 0

/*
 * the function save the copybook and all it's data
 * if save_audio == true -> save also the audio
*/

int savefile::savefile_check_file(){
    setCurrentVersion(currenttitle);

    int error, temp, len, i, check;
    zip_error_t errore;

    zip_t *filezip = zip_open(path->toUtf8().constData(), ZIP_CREATE, &error);

    if(!filezip)
        return ERROR;


    zip_source_t *file;
    file = zip_source_buffer_create(0, 0, 0, &errore);
    if(!file){
        zip_close(filezip);
        return ERROR;
    }

    zip_source_begin_write(file);

    SOURCE_WRITE(file, &currenttitle->versione, sizeof(int))

    SAVE_STRINGA(file, currenttitle->nome_copybook.toUtf8().constData());

    {
        int temp = static_cast<int>(currenttitle->se_registato);
        SOURCE_WRITE(file, &temp, sizeof(int))
    }

    SOURCE_WRITE(file, &currenttitle->se_tradotto, sizeof(bool))

    SAVE_STRINGA(file, currenttitle->testi.toUtf8().constData());

    SAVE_STRINGA(file, currenttitle->audio_position_path.toUtf8().constData())

    SOURCE_WRITE(file, &currenttitle->m_touch, sizeof(bool))

    if(currenttitle->m_touch)
        SAVE_BINARY(filezip);

    /* testinohtml */
    len = currenttitle->testinohtml.length();
    SOURCE_WRITE(file, &len, sizeof(int))
    for(i=0; i<len; i++)
        SAVE_STRINGA(file, currenttitle->testinohtml.at(i).toUtf8().constData())


    /* posizione_iniz */
    for(i=0; i<len; i++){
        temp = currenttitle->posizione_iniz.at(i);
        SOURCE_WRITE(file, &temp, sizeof(int))
    }


    SAVE_IMAGE(&currenttitle->immagini, file)

    check = 0;

    /*
     * Upon successful completion 0 is returned. Otherwise, -1 is returned
     * and the error information in source is set to indicate the error.
    */
    check += zip_source_commit_write(file)==ERROR_PRIVATE;

    /*
     * Upon successful completion, zip_file_add() returns the index of
     * the new file in the archive, and zip_file_replace() returns 0.
     * Otherwise, -1 is returned and the error code in archive is set
     * to indicate the error.
    */
    check += zip_file_add(filezip,
                 (currenttitle->nome_copybook + (QString)".xml").toUtf8().constData(),
                 file,
                 ZIP_FL_OVERWRITE)==ERROR_PRIVATE;

    if(check != OK_PRIVATE)
        goto delete_;


    zip_close(filezip);
    return OK;

    delete_:
        zip_source_free(file);
        zip_close(filezip);
        return ERROR;
}

/*
 * return ERROR or OK
*/
static int save_string(zip_source_t *file, const char *stringa){
    int size = strlen(stringa);
    SOURCE_WRITE_RETURN(file, &size, sizeof(int));

    SOURCE_WRITE_RETURN(file, stringa, sizeof(char)*size);

    return OK;

}

int save_audio_file(QByteArray &array, QString &namecopybook, QString &path){
    zip_source_t *file;
    zip_t *filezip;

    zip_error_t errore;
    int check, error;

    filezip = zip_open(path.toUtf8().constData(), ZIP_CREATE, &error);

    if(!filezip)
        return ERROR;

    file = zip_source_buffer_create(0, 0, 0, &errore);
    if(!file){
        zip_close(filezip);
        return ERROR;
    }

    zip_source_begin_write(file);

    void *data = array.data();
    int size = array.size();

    SOURCE_WRITE_RETURN(file, &size, sizeof(int));

    SOURCE_WRITE(file, data, size);

    check = 0;

    check += zip_source_commit_write(file)==ERROR_PRIVATE;

    check += zip_file_add(filezip,
                 (namecopybook + "audio.wav").toUtf8().constData(),
                 file,
                 ZIP_FL_OVERWRITE)==ERROR_PRIVATE;

    if(check != OK_PRIVATE)
        goto delete_;

    return OK;

    delete_:
    zip_source_free(file);
    return ERROR;
}

static void setCurrentVersion(currenttitle_class *data){
    data->versione = CURRENT_VERSION_CURRENT_TITLE;
}
