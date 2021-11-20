#include "xmlstruct.h"
#include "../currenttitle/document.h"
#include "../images/fromimage.h"
#include <zip.h>
#include "../datawrite/source_read_ext.h"
#include "../utils/posizione_binario.h"
#include "../sheet/fast-sheet/fast_sheet_ui.h"

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

    QList<double> pos_foglio;
    QList<point_s> point;
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(z, NAME_BIN, 0, &st);

    zip_file_t *f = zip_fopen(z, NAME_BIN, 0);

    if(f == nullptr)
        return ERROR;

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

        point.append(temp_point);
    }

    double valoretemp;

    SOURCE_READ_GOTO(f, &len, sizeof(int));
    for(i=0; i < len; i++){
        SOURCE_READ_GOTO(f, &valoretemp, sizeof(double));
        pos_foglio.append(valoretemp);
    }
    
    SOURCE_READ_GOTO(f, &this->currenttitle->datatouch->zoom, sizeof(long double));

    zip_fclose(f);

    xmlstruct::decode(currenttitle, point, pos_foglio);

    return OK;

    free_:
    zip_fclose(f);
    return ERROR;
}

struct point_last{
    double m_x, m_y, rotation;
    float m_pressure;
    int m_posizioneaudio;
    struct colore_s m_color;
    int idtratto;

    size_t createControll() const;
    bool isIdUser() const;
};

static size_t createControllOldVersion(const QList<point_last> &point)
{
    uint i;
    const uint len = point.length();
    size_t controll = 0;
    for(i = 0; i < len; i++){
        controll += point.at(i).createControll();
    }

    return controll;
}

int xmlstruct::loadbinario_1(struct zip *z){

    struct zip_stat st;
    size_t controll;
    int i, len;
    zip_file_t *f;
    struct point_last temp_point;
    double valoretemp;

    QList<point_s> __tmp;
    point_s __point;

    QList<point_last> point;
    QList<double> pos_foglio;

    zip_stat_init(&st);
    zip_stat(z, NAME_BIN, 0, &st);

     f = zip_fopen(z, NAME_BIN, 0);

    if(f == nullptr)
        return false;


    SOURCE_READ_GOTO(f, &len, sizeof(int));

    for(i=0; i<len; i++){
        SOURCE_READ_GOTO(f, &temp_point, sizeof(struct point_last));
        point.append(temp_point);
    }

    SOURCE_READ_GOTO(f, &len, sizeof(int));
    for(i=0; i < len; i++){
        SOURCE_READ_GOTO(f, &valoretemp, sizeof(double));
        pos_foglio.append(valoretemp);
    }

    SOURCE_READ_GOTO(f, &this->currenttitle->datatouch->zoom, sizeof(this->currenttitle->datatouch->zoom));

    SOURCE_READ_GOTO(f, &controll, sizeof(size_t));

    zip_fclose(f);

    if(controll != createControllOldVersion(point))
        return ERROR_CONTROLL;

    for(const auto & ref : qAsConst(point)){
        memcpy(&__point.m_color, &ref.m_color, sizeof(colore_s));
        __point.idtratto = ref.idtratto;
        __point.m_posizioneaudio = ref.m_posizioneaudio;
        __point.m_pressure = ref.m_pressure;
        __point.m_x = ref.m_x;
        __point.m_y = ref.m_y;
        __point.rotation = ref.rotation;

        __tmp.append(__point);
    }

    xmlstruct::decode(currenttitle, __tmp, pos_foglio);
    return OK;

    free_:
    zip_fclose(f);
    return ERROR;
}

#endif //ALL_VERSION

int xmlstruct::loadbinario_2(struct zip *z){
    struct zip_stat st;
    size_t controll, newControll;
    int i, len, lenPage, counterPage;
    zip_file_t *f;
    page *page;
    struct point_s temp_point;
    double init[2];

    zip_stat_init(&st);
    zip_stat(z, NAME_BIN, 0, &st);

     f = zip_fopen(z, NAME_BIN, 0);

    if(f == nullptr) return ERROR;

    /* point first page */
    SOURCE_READ_GOTO(f, init, sizeof(double)*2);
    this->currenttitle->datatouch->setPointFirstPage(QPointF(init[0], init[1]));

    /* page len */
    SOURCE_READ_GOTO(f, &lenPage, sizeof(lenPage));
    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        SOURCE_READ_GOTO(f, &len, sizeof(len));
        currenttitle->datatouch->newPage(n_style::white);
        page = currenttitle->datatouch->at_mod(counterPage);

        for(i=0; i<len; i++){
            SOURCE_READ_GOTO(f, &temp_point, sizeof(struct point_s));
            page->append(temp_point);
        }
    }

    SOURCE_READ_GOTO(f, &this->currenttitle->datatouch->zoom, sizeof(this->currenttitle->datatouch->zoom));

    SOURCE_READ_GOTO(f, &controll, sizeof(size_t));

    zip_fclose(f);

    currenttitle->datatouch->triggerNewView(-1, true);
    newControll = currenttitle->createSingleControll();

    if(controll != newControll)
        return ERROR_CONTROLL;

    return OK;

    free_:
    zip_fclose(f);
    return ERROR;
}

size_t point_last::createControll() const{
    static size_t data;
    static int i;

    data = 0;
    data += m_x;
    data += m_y;
    data += m_pressure;
    data += idtratto;

    for(i=0; i<NCOLOR; ++i){
        data += m_color.colore[i];
    }

    data += this->rotation;
    data += this->m_posizioneaudio;

    return data;
}
