#include "xmlstruct.h"

#include "../currenttitle/currenttitle_class.h"
#include "../images/save_images.h"
#include <zip.h>

#include "../datawrite/source_read_ext.h"

#define SOURCE_READ_ZIP_PRIVATE(x, y, z) zip_fread(x, y, z)==-1
#define SOURCE_READ_ZIP(x, y, z) check+=SOURCE_READ_ZIP_PRIVATE(x, y, z)

/* la funzione gestisce la lettura del file binario */
bool xmlstruct::loadbinario(zip_t *z){
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(z, this->currenttitle->posizione_binario.toUtf8().constData(), 0, &st);

    zip_file_t *f = zip_fopen(z, this->currenttitle->posizione_binario.toUtf8().constData(), 0);

    int check = 0;

    if(f == nullptr)
        return false;


    int lunghezza=0, i, valoretemp;
    float temp;
    double variabiledouble;

    /* x lenght*/
    SOURCE_READ_ZIP(f, &lunghezza, sizeof(int));
    //zip_fread(f, &lunghezza, sizeof(int));


    for(i=0; i < lunghezza; i++){
        SOURCE_READ_ZIP(f, &variabiledouble, sizeof(double));
        //check += source_read_ext(f, &variabiledouble, sizeof(double));
        this->currenttitle->datatouch->x.append(variabiledouble);
    }

    /* y */
    for(i=0; i < lunghezza; i++){
        SOURCE_READ_ZIP(f, &variabiledouble, sizeof(double));
        //check += source_read_ext(f, &variabiledouble, sizeof(double));
        this->currenttitle->datatouch->y.append(variabiledouble);
    }

    /* idtratto */
    for(i=0; i < lunghezza; i++){
        SOURCE_READ_ZIP(f, &valoretemp, sizeof(int));
        //check += source_read_ext(f, &valoretemp, sizeof(int));
        this->currenttitle->datatouch->idtratto.append(valoretemp);
    }

    /* pressure */
    for(i=0; i < lunghezza; i++){
        SOURCE_READ_ZIP(f, &temp, sizeof(float));
        //check += source_read_ext(f, &temp, sizeof(float));
        this->currenttitle->datatouch->pressure.append(temp);
    }

    /* rotation */
    for(i=0; i < lunghezza; i++){
        SOURCE_READ_ZIP(f, &valoretemp, sizeof(int));
        //check += source_read_ext(f, &valoretemp, sizeof(int));
        this->currenttitle->datatouch->rotation.append(valoretemp);
    }

    /* colori */
    struct colore_s coloretemp;
    for(i = 0; i < lunghezza; i++){
        SOURCE_READ_ZIP(f, &cloretemp, sizeof(struct colore_s));
        //source_read_ext(f, &coloretemp, sizeof(struct colore_s));

        currenttitle->datatouch->color.append(coloretemp);
    }

    /* carica la posizione dei testi */
    for(i=0; i < lunghezza; i++){
        SOURCE_READ_ZIP(f, &valoretemp, sizeof(int));
        check += source_read_ext(f, &valoretemp, sizeof(int));
        this->currenttitle->datatouch->posizioneaudio.append(valoretemp);
    }

    /* posizione foglio */
    SOURCE_READ_ZIP(f, &lunghezza, sizeof(int));
    //check += source_read_ext(f, &lunghezza, sizeof(int));
    for(i=0; i < lunghezza; i++){
        SOURCE_READ_ZIP(f, &valoretemp, sizeof(int));
        //check += source_read_ext(f, &valoretemp, sizeof(int));
        currenttitle->datatouch->posizionefoglio.append(valoretemp);
    }
    
    SOURCE_READ_ZIP(f, &this->currenttitle->datatouch->zoom, sizeof(long double));
    //check += source_read_ext(f, &this->currenttitle->datatouch->zoom, sizeof(long double));

    check += load_image(&currenttitle->datatouch->immagini, f);

    zip_fclose(f);

    return check == 0;
}

