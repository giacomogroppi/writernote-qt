#include "xmlstruct.h"
#include "../currenttitle/document.h"
#include "../images/fromimage.h"
#include <zip.h>
#include "../datawrite/source_read_ext.h"
#include "../utils/posizione_binario.h"
#include "../sheet/fast-sheet/fast_sheet_ui.h"

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
