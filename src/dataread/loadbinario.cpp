#include "xmlstruct.h"

#include "../currenttitle/currenttitle_class.h"
#include "../images/save_images.h"
#include <zip.h>

#include "../datawrite/source_read_ext.h"


#include "../utils/posizione_binario.h"

/* la funzione gestisce la lettura del file binario */
int xmlstruct::loadbinario(zip_t *z){
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(z, POSIZIONEBINARIO(currenttitle->nome_copybook), 0, &st);

    zip_file_t *f = zip_fopen(z, POSIZIONEBINARIO(this->currenttitle->nome_copybook), 0);

    //int check = 0;

    if(f == nullptr)
        return false;


    int lunghezza=0, i, valoretemp;
    float temp;
    double variabiledouble;

    /* x lenght*/
    SOURCE_READ(f, &lunghezza, sizeof(int));
    //zip_fread(f, &lunghezza, sizeof(int));


    for(i=0; i < lunghezza; i++){
        SOURCE_READ(f, &variabiledouble, sizeof(double));
        //check += source_read_ext(f, &variabiledouble, sizeof(double));
        this->currenttitle->datatouch->x.append(variabiledouble);
    }

    /* y */
    for(i=0; i < lunghezza; i++){
        SOURCE_READ(f, &variabiledouble, sizeof(double));
        //check += source_read_ext(f, &variabiledouble, sizeof(double));
        this->currenttitle->datatouch->y.append(variabiledouble);
    }

    /* idtratto */
    for(i=0; i < lunghezza; i++){
        SOURCE_READ(f, &valoretemp, sizeof(int));
        //check += source_read_ext(f, &valoretemp, sizeof(int));
        this->currenttitle->datatouch->idtratto.append(valoretemp);
    }

    /* pressure */
    for(i=0; i < lunghezza; i++){
        SOURCE_READ(f, &temp, sizeof(float));
        this->currenttitle->datatouch->pressure.append(temp);
    }

    /* rotation */
    for(i=0; i < lunghezza; i++){
        SOURCE_READ(f, &valoretemp, sizeof(int));
        this->currenttitle->datatouch->rotation.append(valoretemp);
    }

    /* colori */
    struct colore_s coloretemp;
    for(i = 0; i < lunghezza; i++){
        SOURCE_READ(f, &coloretemp, sizeof(struct colore_s));
        currenttitle->datatouch->color.append(coloretemp);
    }

    /* carica la posizione dei testi */
    for(i=0; i < lunghezza; i++){
        SOURCE_READ(f, &valoretemp, sizeof(int));
        this->currenttitle->datatouch->posizioneaudio.append(valoretemp);
    }

    /* posizione foglio */
    SOURCE_READ(f, &lunghezza, sizeof(int));
    for(i=0; i < lunghezza; i++){
        SOURCE_READ(f, &valoretemp, sizeof(int));
        currenttitle->datatouch->posizionefoglio.append(valoretemp);
    }
    
    SOURCE_READ(f, &this->currenttitle->datatouch->zoom, sizeof(long double));

    zip_fclose(f);

    return OK;

    free_:
    zip_fclose(f);
    return ERROR;
}

