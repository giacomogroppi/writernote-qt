#include "../savefile.h"
#include "../source_read_ext.h"

#define SOURCE_WRITE(x, y, z) if(zip_source_write(x, y, z)==-1) goto delete_;
#define SAVE_BINARY(x) if(salvabinario(x)==false)goto delete_;

bool savefile::savefile_check_file(){
    int error, temp, len, i, check;
    zip_error_t errore;

    zip_t *filezip = zip_open(path->toUtf8().constData(), ZIP_CREATE, &error);

    if(!filezip)
        return false;


    zip_source_t *file;
    file = zip_source_buffer_create(0, 0, 0, &errore);
    if(!file){
        zip_close(filezip);
        return false;
    }

    zip_source_begin_write(file);

    SOURCE_WRITE(file, &currenttitle->versione, sizeof(qint32))

    /* name copybook */
    temp = currenttitle->nome_copybook.length();
    SOURCE_WRITE(file, &temp, sizeof(qint32))
    if(temp)
        SOURCE_WRITE(file, currenttitle->nome_copybook.toUtf8().constData(), sizeof(char)*temp)


    temp = (int)currenttitle->se_registato;
    SOURCE_WRITE(file, &temp, sizeof(qint32))

    temp = (int)currenttitle->se_tradotto;
    SOURCE_WRITE(file, &temp, sizeof(qint32))

    temp = currenttitle->testi.length();
    SOURCE_WRITE(file, &temp, sizeof(qint32));
    SOURCE_WRITE(file, currenttitle->testi.toUtf8().constData(), sizeof(char)*temp)

    temp = currenttitle->audio_position_path.length();
    SOURCE_WRITE(file, &temp, sizeof(qint32))

    SOURCE_WRITE(file, currenttitle->audio_position_path.toUtf8().constData(), sizeof(char)*temp)

    SOURCE_WRITE(file, &currenttitle->m_touch, sizeof(qint32))

    if(temp) {
        if(currenttitle->m_touch)
            SAVE_BINARY(filezip);
    }


    // testinohtml
    len = currenttitle->testinohtml.length();
    SOURCE_WRITE(file, &len, sizeof(qint32))

    for(i=0; i<len; i++){
        temp = currenttitle->testinohtml.at(i).length();
        SOURCE_WRITE(file, &temp, sizeof(qint32))

        SOURCE_WRITE(file, currenttitle->testinohtml.at(i).toUtf8().constData(), sizeof(char)*temp)
    }

    for(i=0; i<len; i++){
        temp = currenttitle->posizione_iniz.at(i);
        SOURCE_WRITE(file, &temp, sizeof(qint32))
    }

    check = 0;

#define ERROR -1
    /*
     * Upon successful completion 0 is returned. Otherwise, -1 is returned
     * and the error information in source is set to indicate the error.
    */
    check += zip_source_commit_write(file)==ERROR;

    /*
     * Upon successful completion, zip_file_add() returns the index of
     * the new file in the archive, and zip_file_replace() returns 0.
     * Otherwise, -1 is returned and the error code in archive is set
     * to indicate the error.
    */
    check += zip_file_add(filezip,
                 (currenttitle->nome_copybook + (QString)".xml").toUtf8().constData(),
                 file,
                 ZIP_FL_OVERWRITE)==ERROR;

    if(check != 0)
        goto delete_;


    zip_close(filezip);
    return true;

    delete_:
        zip_source_free(file);
        zip_close(filezip);
        return false;
}
