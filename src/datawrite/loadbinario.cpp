#include "xmlstruct.h"

#include "../currenttitle/currenttitle_class.h"

#include <zip.h>

/* la funzione gestisce la lettura del file binario */
void xmlstruct::loadbinario(){
    int err = 0;
    zip *z = zip_open(this->path_->c_str(), 0, &err);

    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(z, this->currenttitle->posizione_binario.toUtf8().constData(), 0, &st);

    zip_file *f = zip_fopen(z, this->currenttitle->posizione_binario.toUtf8().constData(), 0);

    if(f == nullptr){
        qDebug() << "xmlstruct::loadbinario -> File impossibile da leggere";
        return;
    }

    int lunghezza=0, i, valoretemp;
    float temp;
    double variabiledouble;

    /* x */
    zip_fread(f, &lunghezza, sizeof(int));

    qDebug() << "Loadbinario -> lunghezza x -> " << lunghezza;

    for(i=0; i < lunghezza; i++){
        zip_fread(f, &variabiledouble, sizeof(double));
        this->currenttitle->datatouch->x.append(variabiledouble);
    }

    /* y */
    for(i=0; i < lunghezza; i++){
        zip_fread(f, &variabiledouble, sizeof(double));
        this->currenttitle->datatouch->y.append(variabiledouble);
    }

    /* idtratto */
    for(i=0; i < lunghezza; i++){
        zip_fread(f, &valoretemp, sizeof(int));
        this->currenttitle->datatouch->idtratto.append(valoretemp);
    }

    /* pressure */
    for(i=0; i < lunghezza; i++){
        zip_fread(f, &temp, sizeof(float));
        this->currenttitle->datatouch->pressure.append(temp);
    }

    /* rotation */
    for(i=0; i < lunghezza; i++){
        zip_fread(f, &valoretemp, sizeof(int));
        this->currenttitle->datatouch->rotation.append(valoretemp);
    }

    /* posizione foglio */
    for(i=0; i < lunghezza; i++){
        zip_fread(f, &valoretemp, sizeof(int));
        currenttitle->datatouch->posizionefoglio.append(valoretemp);
    }

    /* colori */
    int pointer[3];
    QColor coloretemp;
    for(i = 0; i < lunghezza; i++){
        zip_fread(f, &pointer[0], sizeof(int));
        zip_fread(f, &pointer[1], sizeof(int));
        zip_fread(f, &pointer[2], sizeof(int));
        coloretemp.setRgb(pointer[0], pointer[1], pointer[2]);

        currenttitle->datatouch->color.append(coloretemp);
    }

    /* carica la posizione dei testi */
    for(i=0; i < lunghezza; i++)
    {
        zip_fread(f, &valoretemp, sizeof(int));
        this->currenttitle->datatouch->posizioneaudio.append(valoretemp);
    }

    zip_fread(f, &this->currenttitle->datatouch->zoom, sizeof(float));

    zip_fclose(f);
    zip_close(z);
}

