#include "xmlstruct.h"
#include "currenttitle/document.h"
#include "images/fromimage.h"
#include <zip.h>
#include "datawrite/source_read_ext.h"
#include "utils/posizione_binario.h"
#include "sheet/fast-sheet/fast_sheet_ui.h"

__new int xmlstruct::loadbinario_3(struct zip *z, int ver_stroke)
{
    struct zip_stat st;
    size_t controll, newControll;
    int len, lenPage, counterPage;
    datastruct *data = currenttitle->datatouch;
    zip_file_t *f;
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

        /* we add a new page */
        data->newPage(n_style::white);

        if(data->at_mod(counterPage).load(f, ver_stroke, len) != OK)
            goto free_;
    }

    SOURCE_READ_GOTO(f, &this->currenttitle->datatouch->zoom, sizeof(this->currenttitle->datatouch->zoom));

    SOURCE_READ_GOTO(f, &controll, sizeof(size_t));

    zip_fclose(f);

    newControll = currenttitle->createSingleControll();

    if(controll != newControll)
        return ERROR_CONTROLL;

    return OK;

    free_:
    zip_fclose(f);
    return ERROR;
}
