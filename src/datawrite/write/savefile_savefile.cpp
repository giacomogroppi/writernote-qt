#include "../savefile.h"
#include "../source_read_ext.h"
#include "../../utils/common_error_definition.h"
#include "../../images/save_images.h"

int savefile::savefile_check_file(){
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

    /* name copybook */
    temp = currenttitle->nome_copybook.length();
    SOURCE_WRITE(file, &temp, sizeof(int))
    if(temp)
        SOURCE_WRITE(file, currenttitle->nome_copybook.toUtf8().constData(), sizeof(char)*temp)


    temp = (int)currenttitle->se_registato;
    SOURCE_WRITE(file, &temp, sizeof(int))

    temp = (int)currenttitle->se_tradotto;
    SOURCE_WRITE(file, &temp, sizeof(int))

    temp = currenttitle->testi.length();
    SOURCE_WRITE(file, &temp, sizeof(qint32));
    SOURCE_WRITE(file, currenttitle->testi.toUtf8().constData(), sizeof(char)*temp)

    temp = currenttitle->audio_position_path.length();
    SOURCE_WRITE(file, &temp, sizeof(int))

    SOURCE_WRITE(file, currenttitle->audio_position_path.toUtf8().constData(), sizeof(char)*temp)

    SOURCE_WRITE(file, &currenttitle->m_touch, sizeof(int))

    if(temp) {
        if(currenttitle->m_touch)
            SAVE_BINARY(filezip);
    }


    // testinohtml
    len = currenttitle->testinohtml.length();
    SOURCE_WRITE(file, &len, sizeof(int))

    for(i=0; i<len; i++){
        temp = currenttitle->testinohtml.at(i).length();
        SOURCE_WRITE(file, &temp, sizeof(int))

        SOURCE_WRITE(file, currenttitle->testinohtml.at(i).toUtf8().constData(), sizeof(char)*temp)
    }

    for(i=0; i<len; i++){
        temp = currenttitle->posizione_iniz.at(i);
        SOURCE_WRITE(file, &temp, sizeof(int))
    }

    /* save images */
    if(save_image(&currenttitle->immagini, file)!=OK)
        goto delete_;

    check = 0;

#define ERROR_PRIVATE -1
#define OK_PRIVATE 0
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
