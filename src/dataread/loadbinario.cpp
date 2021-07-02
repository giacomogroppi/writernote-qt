#include "xmlstruct.h"

#include "../currenttitle/document.h"
#include "../images/save_images.h"
#include <zip.h>

#include "../datawrite/source_read_ext.h"


#include "../utils/posizione_binario.h"

#ifdef ALL_VERSION

/* la funzione gestisce la lettura del file binario */
int xmlstruct::loadbinario_0(zip_t *z){
    struct colore_last{
        int colore[NCOLOR];
    };

    struct point_last{
        double m_x, m_y, rotation;
        float m_pressure;
        int m_posizioneaudio;
        struct colore_last m_color;
        int idtratto;

        size_t createControll() const;
        bool isIdUser() const;
    };

    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(z, POSIZIONEBINARIO(currenttitle->nome_copybook), 0, &st);

    zip_file_t *f = zip_fopen(z, POSIZIONEBINARIO(this->currenttitle->nome_copybook), 0);

    if(f == nullptr)
        return false;

    int i, len;
    uint k;
    SOURCE_READ_GOTO(f, &len, sizeof(int));

    struct point_last point_lettura;
    struct point_s temp_point;

    for(i=0; i<len; i++){
        SOURCE_READ_GOTO(f, &point_lettura, sizeof(struct point_last));

        temp_point.idtratto = point_lettura.idtratto;
        for(k=0; k<NCOLOR; ++k){
            temp_point.m_color.colore[k] = point_lettura.m_color.colore[k];
        }

        temp_point.m_posizioneaudio = point_lettura.m_posizioneaudio;
        temp_point.m_x = point_lettura.m_x;
        temp_point.m_y = point_lettura.m_y;
        temp_point.m_pressure = point_lettura.m_pressure;
        temp_point.rotation = point_lettura.rotation;

        currenttitle->datatouch->append(temp_point);
    }

    double valoretemp;
    
    SOURCE_READ_GOTO(f, &len, sizeof(int));
    for(i=0; i < len; i++){
        SOURCE_READ_GOTO(f, &valoretemp, sizeof(double));
        currenttitle->datatouch->posizionefoglio.append(valoretemp);
    }
    
    SOURCE_READ_GOTO(f, &this->currenttitle->datatouch->zoom, sizeof(long double));

    zip_fclose(f);

    return OK;

    free_:
    zip_fclose(f);
    return ERROR;
}

#endif

int xmlstruct::loadbinario_1(struct zip *z){
    struct zip_stat st;
    size_t controll;

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
        currenttitle->datatouch->append(temp_point);
    }

    double valoretemp;

    SOURCE_READ_GOTO(f, &len, sizeof(int));
    for(i=0; i < len; i++){
        SOURCE_READ_GOTO(f, &valoretemp, sizeof(double));
        currenttitle->datatouch->posizionefoglio.append(valoretemp);
    }

    SOURCE_READ_GOTO(f, &this->currenttitle->datatouch->zoom, sizeof(long double));

    SOURCE_READ_GOTO(f, &controll, sizeof(size_t));

    zip_fclose(f);

    if(controll != currenttitle->createSingleControll())
        return ERROR_CONTROLL;

    return OK;

    free_:
    zip_fclose(f);
    return ERROR;
}
