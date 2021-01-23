#include "../xmlstruct.h"

#include "../../images/save_images.h"
#include "../source_read_ext.h"

static int load_stringa(zip_file_t *f, QString *stringa){
    /* return 0 if all is ok, else return -1 */
    int check = 0, temp;

    // legge quanto è lungo il nome del file
    check += source_read_ext(f, &temp, sizeof(int));

    if(temp){
        char *vartempp = new char[temp + 1];

        check += source_read_ext(f, vartempp, sizeof(char)*temp);

        vartempp[temp] = '\0';

        *stringa = vartempp;

        delete [] vartempp;

    }

    return check;
}

static int load_multiplestring(zip_file_t *f, QList<QString> * lista, QList<int> * data){
    int check = 0, i, lunghezza, temp;
    //testinohtml
    check += source_read_ext(f, &lunghezza, sizeof(int));

    if(lunghezza){
        char *variabiletemp;

        for(i=0; i<lunghezza; i++){
            /*lista->append("");

            check += load_stringa(f, (QString *)&lista->at(i));*/

            check += source_read_ext(f, &temp, sizeof(int));


            variabiletemp = new char[temp + 1];

            check += source_read_ext(f, variabiletemp, sizeof(char)*temp);

            variabiletemp[temp] = '\0';

            lista->append(variabiletemp);
            delete [] variabiletemp;
        }
    }

    for(i=0; i<lunghezza; i++){
        check += source_read_ext(f, &temp, sizeof(int));

        data->append(temp);
    }

    return check;
}

bool xmlstruct::loadfile(const char *nameFile){
    currenttitle->reset();
    currenttitle->datatouch->reset();

    int err = 0, check = 0;
    int temp;

    zip *filezip = zip_open(this->path_->toUtf8().constData(), 0, &err);
    if (filezip == NULL)
        return false;

    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(filezip, nameFile, 0, &st);

    zip_file *f = zip_fopen(filezip, nameFile, 0);
    if(f == NULL){
        zip_close(filezip);
        return false;
    }

    check += source_read_ext(f, &currenttitle->versione, sizeof(int));
    check += source_read_ext(f, &temp, sizeof(int));
    currenttitle->se_registato = temp;

    check += source_read_ext(f, &temp, sizeof(int));
    currenttitle->se_tradotto = temp;

    check += load_stringa(f, &currenttitle->testi);

    check += load_stringa(f, &currenttitle->audio_position_path);

    check += load_stringa(f, &currenttitle->posizione_binario);
    if(currenttitle->posizione_binario != ""){
        if(!this->loadbinario(filezip)){
            zip_fclose(f);
            zip_close(filezip);
            return false;
        }
    }

    check += load_multiplestring(f, &currenttitle->testinohtml, &currenttitle->posizione_iniz);

    if(currenttitle->posizione_binario == ""){
        check += load_image(&currenttitle->immagini, f);
    }

    zip_fclose(f);
    zip_close(filezip);

    return check == 0;
}
