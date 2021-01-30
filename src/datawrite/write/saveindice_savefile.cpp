#include "../savefile.h"

#include "../../indice_class.h"

/* funzione che gestisce la creazione di una stringa per salvare l'indice */
bool savefile::savefile_check_indice(indice_class *indice){
    int error, temp, len, i, check;
    zip_error_t errore;

    check = 0;

    zip_t *filezip = zip_open(path->toUtf8().constData(), ZIP_CREATE, &error);

    zip_source_t *file;
    file = zip_source_buffer_create(0, 0, 0, &errore);
    zip_source_begin_write(file);

    check += zip_source_write(file, &indice->versione, sizeof(int));

    len = indice->titolo.length();

    check += zip_source_write(file, &len, sizeof(int));

    char *nome;

    for(i=0; i<len; i++){
        temp = indice->titolo.at(i).length();
        zip_source_write(file, &temp, sizeof(int));

        nome = (char *)indice->titolo.at(i).toUtf8().constData();
        zip_source_write(file, nome, sizeof(char)*temp);
    }

    check += zip_source_commit_write(file);
    check += zip_file_add(filezip,
                 "indice.xml",
                 file,
                 ZIP_FL_OVERWRITE);

    zip_close(filezip);

    if(check < 0)
        return false;

    return true;
}
