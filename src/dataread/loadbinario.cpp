#include "xmlstruct.h"
#include "currenttitle/document.h"
#include <zip.h>
#include "sheet/fast-sheet/fast_sheet_ui.h"
#include "utils/common_script.h"
#include "core/WReadZip.h"
#include <pthread.h>

static int load_point_first_page(WZip &zip, size_t &seek_from, datastruct &doc)
{
    double init[2];
    /* point first page */
    seek_from += WMemcpy(init, zip.get_data(), sizeof(double) * 2);
    doc.setPointFirstPage(QPointF(init[0], init[1]));

    W_ASSERT(0);

    return 0;
}

static int read_number_page(WZip &zip, int &len, size_t &seek_from, datastruct &doc)
{
    /* page len */
    static_assert(sizeof(len) == sizeof(int));
    seek_from += WMemcpy(&len, zip.get_data() + seek_from, sizeof(len));
    W_ASSERT(len > 0);

    return 0;
}

static int read_zoom(WZip &zip, double &zoom, size_t &seek_from)
{
    static_assert(sizeof(zoom) == sizeof(double));
    seek_from += WMemcpy(&zoom, zip.get_data() + seek_from, sizeof(zoom));
    return 0;
}

static int read_ctrl(WZip &zip, size_t &ctrl, size_t &seek_from)
{
    static_assert(sizeof(ctrl) == sizeof(size_t));
    seek_from += WMemcpy(&ctrl, zip.get_data() + seek_from, sizeof(ctrl));
    return 0;
}

struct xmlstruct_thread_data{
    WReadZip    *_zip;
    int         _id;
    page        *_page;
    int         _ver_stroke;
};

static void *xmlstruct_thread_load(void *_data)
{
    int i;
    struct xmlstruct_thread_data *data = static_cast<struct xmlstruct_thread_data *>(_data);

    if(data->_page->load(*data->_zip, data->_ver_stroke, data->_id) != OK)
        return (void *)1UL;

    return 0;
}

#define MANAGE_ERR()    \
    do{                 \
        return ERROR;   \
    }while(0)

int xmlstruct::loadbinario_4(class WZip &zip, int ver_stroke)
{
    size_t controll, newControll;
    int lenPage, counterPage;
    datastruct *data = currenttitle->datatouch;

    size_t needToSeekFrom = 0;

    if(!zip.openFileInZip(NAME_BIN))
        return ERROR;

    if(load_point_first_page(zip, needToSeekFrom, *this->currenttitle->datatouch))
        MANAGE_ERR();

    if(read_number_page(zip, lenPage, needToSeekFrom, *currenttitle->datatouch))
        MANAGE_ERR();

    if(read_zoom(zip, currenttitle->datatouch->_zoom, needToSeekFrom))
        MANAGE_ERR();

    if(read_ctrl(zip, controll, needToSeekFrom))
        MANAGE_ERR();

    size_t seek[lenPage];
    pthread_t thread[lenPage];
    struct xmlstruct_thread_data thread_data[lenPage];

    needToSeekFrom += WMemcpy(seek, zip.get_data() + needToSeekFrom, sizeof(size_t) * lenPage);

    WReadZip zipReader(&zip, lenPage, needToSeekFrom, seek);

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        data->newPage(n_style::white);

        thread_data[counterPage] = {
            ._zip           = &zipReader,
            ._id            = counterPage,
            ._page          = &currenttitle->datatouch->at_mod(counterPage),
            ._ver_stroke    = ver_stroke
        };

        pthread_create( &thread[counterPage],
                        NULL,
                        xmlstruct_thread_load,
                        &thread_data[counterPage]);
    }

    zip.dealloc_file();

    newControll = currenttitle->createSingleControll();

    if(controll != newControll)
        return ERROR_CONTROLL;

    return OK;
}
