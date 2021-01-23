#include "../savefile.h"
#include "../source_read_ext.h"

bool savefile::savefile_check_file(int posizione){
    int error, temp, len, i, check;
    zip_error_t errore;

    check = 0;

    zip_t *filezip = zip_open(parent->self->path.toUtf8().constData(), ZIP_CREATE, &error);

    if(!filezip){
        return false;
    }

    zip_source_t *file;
    file = zip_source_buffer_create(0, 0, 0, &errore);
    if(!file){
        zip_close(filezip);
        return false;
    }

    zip_source_begin_write(file);

    check += source_write_ext(file, &currenttitle->versione, sizeof(int));

    temp = (int)currenttitle->se_registato;
    check += source_write_ext(file, &temp, sizeof(int));

    temp = (int)currenttitle->se_tradotto;
    check += source_write_ext(file, &temp, sizeof(int));

    temp = currenttitle->testi.length();
    check += source_write_ext(file, &temp, sizeof(int));
    check += source_write_ext(file, currenttitle->testi.toUtf8().constData(), sizeof(char)*temp);

    temp = currenttitle->audio_position_path.length();
    check += source_write_ext(file, &temp, sizeof(int));
    check += source_write_ext(file, currenttitle->audio_position_path.toUtf8().constData(), sizeof(char)*temp);

    temp = currenttitle->posizione_binario.length();
    check += source_write_ext(file, &temp, sizeof(int));
    if(temp) {
        check += source_write_ext(file, currenttitle->posizione_binario.toUtf8().constData(), sizeof(char)*temp);

        if(currenttitle->posizione_binario != "")
            this->salvabinario(posizione, filezip);
    }

    // testinohtml
    len = currenttitle->testinohtml.length();
    check += source_write_ext(file, &len, sizeof(int));

    for(i=0; i<len; i++){
        temp = currenttitle->testinohtml.at(i).length();
        check += source_write_ext(file, &temp, sizeof(int));

        check += source_write_ext(file, currenttitle->testinohtml.at(i).toUtf8().constData(), sizeof(char)*temp);
    }

    for(i=0; i<len; i++){
        temp = currenttitle->posizione_iniz.at(i);
        check += source_write_ext(file, &temp, sizeof(int));
    }

    check += zip_source_commit_write(file);
    check += zip_file_add(filezip,
                 (parent->self->indice.titolo.at(posizione) + (QString)".xml").toUtf8().constData(),
                 file,
                 ZIP_FL_OVERWRITE);



    if(check < 0){
        zip_source_free(file);
        zip_close(filezip);
        return false;
    }

    zip_close(filezip);
    return true;
}
