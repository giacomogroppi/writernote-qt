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

    if(f == nullptr)
        return false;

    int i, len;
    SOURCE_READ_GOTO(f, &len, sizeof(int));

    struct point_s temp_point;
    for(i=0; i<len; i++){
        SOURCE_READ_GOTO(f, &temp_point, sizeof(struct point_s));
        currenttitle->datatouch->m_point.append(temp_point);
    }

    /*
     * for the last data struct
    */
    /*
    int lunghezza=0, i, valoretemp;
    float temp;
    double variabiledouble;

    SOURCE_READ(f, &lunghezza, sizeof(int));


    for(i=0; i < lunghezza; i++){
        SOURCE_READ_GOTO(f, &variabiledouble, sizeof(double));
        this->currenttitle->datatouch->x.append(variabiledouble);
    }


    for(i=0; i < lunghezza; i++){
        SOURCE_READ_GOTO(f, &variabiledouble, sizeof(double));
        this->currenttitle->datatouch->y.append(variabiledouble);
    }

    for(i=0; i < lunghezza; i++){
        SOURCE_READ_GOTO(f, &valoretemp, sizeof(int));
        this->currenttitle->datatouch->idtratto.append(valoretemp);
    }

    for(i=0; i < lunghezza; i++){
        SOURCE_READ_GOTO(f, &temp, sizeof(float));
        this->currenttitle->datatouch->pressure.append(temp);
    }

    for(i=0; i < lunghezza; i++){
        SOURCE_READ_GOTO(f, &valoretemp, sizeof(int));
        this->currenttitle->datatouch->rotation.append(valoretemp);
    }

    struct colore_s coloretemp;
    for(i = 0; i < lunghezza; i++){
        SOURCE_READ_GOTO(f, &coloretemp, sizeof(struct colore_s));
        currenttitle->datatouch->color.append(coloretemp);
    }

    for(i=0; i < lunghezza; i++){
        SOURCE_READ_GOTO(f, &valoretemp, sizeof(int));
        this->currenttitle->datatouch->posizioneaudio.append(valoretemp);
    }
    */
    double valoretemp;
    
    SOURCE_READ_GOTO(f, &len, sizeof(int));
    for(i=0; i < len; i++){
        SOURCE_READ_GOTO(f, &valoretemp, sizeof(double));
        currenttitle->datatouch->posizionefoglio.append(valoretemp);
    }
    
    SOURCE_READ_GOTO(f, &this->currenttitle->datatouch->zoom, sizeof(double));

    zip_fclose(f);

    return OK;

    free_:
    zip_fclose(f);
    return ERROR;
}

