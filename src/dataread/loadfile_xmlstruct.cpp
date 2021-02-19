#include "xmlstruct.h"

#include "../images/save_images.h"
#include "../datawrite/source_read_ext.h"

#define ERROR 1
#define OK 0

#define SOURCE_READ(x, y, z) if(zip_fread(x, y, z)==-1)return ERROR

static int load_stringa(zip_file_t *f, QString *stringa){
    /* return 0 if all is ok, else return -1 */
    int temp;

    // legge quanto è lungo il nome del file
    SOURCE_READ(f, &temp, sizeof(qint32));
    //check += source_read_ext(f, &temp, sizeof(int));

    if(temp){
        char *vartempp = new char[temp + 1];

        SOURCE_READ(f, vartempp, sizeof(char)*temp);
        //check += source_read_ext(f, vartempp, sizeof(char)*temp);

        vartempp[temp] = '\0';

        *stringa = vartempp;

        delete [] vartempp;

    }

    return OK;
}

static int load_multiplestring(zip_file_t *f, QList<QString> * lista, QList<int> * data){
    int check = 0, i, lunghezza, temp;

    SOURCE_READ(f, &lunghezza, sizeof(qint32));
    //check += source_read_ext(f, &lunghezza, sizeof(int));

    if(lunghezza){
        char *variabiletemp;

        for(i=0; i<lunghezza; i++){
            SOURCE_READ(f, &temp, sizeof(qint32));
            check += source_read_ext(f, &temp, sizeof(int));


            variabiletemp = new char[temp + 1];

            SOURCE_READ(f, variabiletemp, sizeof(char)*temp);
            //check += source_read_ext(f, variabiletemp, sizeof(char)*temp);

            variabiletemp[temp] = '\0';

            lista->append(variabiletemp);
            delete [] variabiletemp;
        }
    }

    for(i=0; i<lunghezza; i++){
        SOURCE_READ(f, &temp, sizeof(qint32));
        //check += source_read_ext(f, &temp, sizeof(int));

        data->append(temp);
    }

    return OK;
}

bool xmlstruct::loadfile(const char *nameFile){
    currenttitle->reset();
    currenttitle->datatouch->reset();

    int err = 0;
    int temp;

    zip_t *filezip = zip_open(this->path_->toUtf8().constData(), 0, &err);
    if (filezip == NULL)
        return false;

    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(filezip, nameFile, 0, &st);

    zip_file_t *f = zip_fopen(filezip, nameFile, 0);
    if(f == NULL){
        zip_close(filezip);
        return false;
    }

    SOURCE_READ(f, &currenttitle->versione, sizeof(qint32));

    if(load_stringa(f, &currenttitle->nome_copybook) != OK)
        return false;

    SOURCE_READ(f, &temp, sizeof(qint32));

    currenttitle->se_registato = temp;

    SOURCE_READ(f, &temp, sizeof(qint32));
    //check += source_read_ext(f, &temp, sizeof(int));
    currenttitle->se_tradotto = temp;

    if(load_stringa(f, &currenttitle->testi) != OK)
        return false;

    if(load_stringa(f, &currenttitle->audio_position_path) != OK)
        return false;


    SOURCE_READ(f, &currenttitle->m_touch, sizeof(bool));

    if(currenttitle->m_touch){
        if(!this->loadbinario(filezip)){
            zip_fclose(f);
            zip_close(filezip);
            return false;
        }
    }

    if(load_multiplestring(f, &currenttitle->testinohtml, &currenttitle->posizione_iniz) != OK)
        return false;

    if(currenttitle->m_touch){
        if(load_image(&currenttitle->immagini, f) != OK)
            return false;
    }

    zip_fclose(f);
    zip_close(filezip);

    return true;
}
