#include "xmlstruct.h"

#include "../images/save_images.h"
#include "../datawrite/source_read_ext.h"
#include "../utils/common_error_definition.h"

#define LOAD_STRINGA(x, y) if(load_stringa(x,y) == ERROR) goto free_;
static int load_stringa(zip_file_t *f, QString *stringa){
    int temp;

    SOURCE_READ(f, &temp, sizeof(int));

    if(temp){
        char *vartempp = new char[temp + 1];

        SOURCE_READ(f, vartempp, sizeof(char)*temp);

        vartempp[temp] = '\0';

        *stringa = vartempp;

        delete [] vartempp;

    }

    return OK;
}

#define LOAD_MULTIPLESTRING(x, y, z) if(load_multiplestring(x,y,z) == ERROR) goto free_;
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

            variabiletemp[temp] = '\0';

            lista->append(variabiletemp);
            delete [] variabiletemp;
        }
    }

    for(i=0; i<lunghezza; i++){
        SOURCE_READ(f, &temp, sizeof(qint32));

        data->append(temp);
    }

    return OK;
}
#define CLOSE_ZIP(x, y) zip_fclose(x);zip_close(y);
#define LOAD_IMAGE(x,y) if(load_image(x, y) != OK)goto free_;
#define LOAD_BINARIO(x) if(loadbinario(x) == ERROR) goto free_;
int xmlstruct::loadfile(const char *nameFile){
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

    SOURCE_READ_GOTO(f, &currenttitle->versione, sizeof(int));

    if(currenttitle->isOkVersion())
        goto error_version;

    LOAD_STRINGA(f, &currenttitle->nome_copybook)

    SOURCE_READ_GOTO(f, &currenttitle->se_registato, sizeof(bool));

    SOURCE_READ_GOTO(f, &currenttitle->se_tradotto, sizeof(bool));

    LOAD_STRINGA(f, &currenttitle->testi)

    LOAD_STRINGA(f, &currenttitle->audio_position_path)

    SOURCE_READ_GOTO(f, &currenttitle->m_touch, sizeof(bool));

    if(currenttitle->m_touch){
        LOAD_BINARIO(filezip);
    }

    LOAD_MULTIPLESTRING(f, &currenttitle->testinohtml, &currenttitle->posizione_iniz)

    LOAD_IMAGE(&currenttitle->immagini, f);

    CLOSE_ZIP(f, filezip);
    return OK;


    free_:
    CLOSE_ZIP(f, filezip);
    return ERROR;

    /*
     * in case we can not operate with the file because it's too old
    */
    error_version:
    CLOSE_ZIP(f, filezip);
    return ERROR_VERSION;
}
