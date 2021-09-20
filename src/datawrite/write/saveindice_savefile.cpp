#include "../savefile.h"
#include "../../indice_class.h"
#include "../source_read_ext.h"

#ifdef val

/* funzione che gestisce la creazione di una stringa per salvare l'indice */
int savefile::savefile_check_indice(indice_class *indice){
    int error, temp, len, i, check;
    zip_error_t errore;

    zip_t *filezip = zip_open(path->toUtf8().constData(), ZIP_CREATE, &error);

    zip_source_t *file;
    file = zip_source_buffer_create(0, 0, 0, &errore);
    if(!file)
        return ERROR;

    zip_source_begin_write(file);

    SOURCE_WRITE(file, &indice->versione, sizeof(indice->versione));

    len = indice->titolo.length();
    SOURCE_WRITE(file, &len, sizeof(int));

    for(i=0; i<len; i++){
        if(savefile::save_string(file, indice->titolo.at(i).toUtf8().constData()) != OK){
            goto delete_;
        }

        /*temp = indice->titolo.at(i).length();
        zip_source_write(file, &temp, sizeof(int));

        nome = (char *)indice->titolo.at(i).toUtf8().constData();
        zip_source_write(file, nome, sizeof(char)*temp);*/
    }

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
                 "indice.xml",
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
#endif
