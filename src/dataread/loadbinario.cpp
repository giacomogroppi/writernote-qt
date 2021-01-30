#include "xmlstruct.h"

#include "../currenttitle/currenttitle_class.h"
#include "../images/save_images.h"
#include <zip.h>

#include "../datawrite/source_read_ext.h"

/* la funzione gestisce la lettura del file binario */
bool xmlstruct::loadbinario(zip_t *z){
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(z, this->currenttitle->posizione_binario.toUtf8().constData(), 0, &st);

    zip_file *f = zip_fopen(z, this->currenttitle->posizione_binario.toUtf8().constData(), 0);

    int check = 0;

    if(f == nullptr)
        return false;


    int lunghezza=0, i, valoretemp;
    float temp;
    double variabiledouble;

    /* x */
    zip_fread(f, &lunghezza, sizeof(int));


    for(i=0; i < lunghezza; i++){
        check += source_read_ext(f, &variabiledouble, sizeof(double));
        this->currenttitle->datatouch->x.append(variabiledouble);
    }

    /* y */
    for(i=0; i < lunghezza; i++){
        check += source_read_ext(f, &variabiledouble, sizeof(double));
        this->currenttitle->datatouch->y.append(variabiledouble);
    }

    /* idtratto */
    for(i=0; i < lunghezza; i++){
        check += source_read_ext(f, &valoretemp, sizeof(int));
        this->currenttitle->datatouch->idtratto.append(valoretemp);
    }

    /* pressure */
    for(i=0; i < lunghezza; i++){
        check += source_read_ext(f, &temp, sizeof(float));
        this->currenttitle->datatouch->pressure.append(temp);
    }

    /* rotation */
    for(i=0; i < lunghezza; i++){
        check += source_read_ext(f, &valoretemp, sizeof(int));
        this->currenttitle->datatouch->rotation.append(valoretemp);
    }

    /* posizione foglio */
    for(i=0; i < lunghezza; i++){
        check += source_read_ext(f, &valoretemp, sizeof(int));
        currenttitle->datatouch->posizionefoglio.append(valoretemp);
    }

    /* colori */
    struct colore_s coloretemp;
    for(i = 0; i < lunghezza; i++){
        /*check += source_read_ext(f, &coloretemp.colore[0], sizeof(int));
        check += source_read_ext(f, &coloretemp.colore[1], sizeof(int));
        check += source_read_ext(f, &coloretemp.colore[2], sizeof(int));
        check += source_read_ext(f, &coloretemp.colore[2], sizeof(int));*/
        source_read_ext(f, &coloretemp, sizeof(struct colore_s));

        currenttitle->datatouch->color.append(coloretemp);
    }

    /* carica la posizione dei testi */
    for(i=0; i < lunghezza; i++){
        check += source_read_ext(f, &valoretemp, sizeof(int));
        this->currenttitle->datatouch->posizioneaudio.append(valoretemp);
    }

    check += source_read_ext(f, &this->currenttitle->datatouch->zoom, sizeof(float));

    check += load_image(&currenttitle->datatouch->immagini, f);

    zip_fclose(f);

    return check == 0;
}

