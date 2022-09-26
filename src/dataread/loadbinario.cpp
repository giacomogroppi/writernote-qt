#include "xmlstruct.h"
#include "currenttitle/document.h"
#include <zip.h>
#include "sheet/fast-sheet/fast_sheet_ui.h"
#include "utils/common_script.h"
#include "core/WReadZip.h"
#include "core/WZipReaderSingle.h"
#include <pthread.h>

static int load_point_first_page(WZipReaderSingle &zip, datastruct &doc)
{
    double init[2];
    /* point first page */

    static_assert(sizeof(init) == sizeof(double) * 2);

    if(zip.read_by_size(init, sizeof(init)) < 0)
        return -1;

    doc.setPointFirstPage(QPointF(init[0], init[1]));

    return 0;
}

static int read_number_page(WZipReaderSingle &zip, int &len, datastruct &doc)
{
    /* page len */
    static_assert(sizeof(len) == sizeof(int));

    if(zip.read_object(len) < 0)
        return -1;

    W_ASSERT(len > 0);

    return 0;
}

static int read_zoom(WZipReaderSingle &zip, double &zoom)
{
    static_assert(sizeof(zoom) == sizeof(double));

    if(zip.read_object(zoom) < 0)
        return -1;

    return 0;
}

static int read_ctrl(WZipReaderSingle &zip, size_t &ctrl)
{
    static_assert(sizeof(ctrl) == sizeof(size_t));
    if(zip.read_object(ctrl) < 0)
        return -1;
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
    struct xmlstruct_thread_data *data = static_cast<struct xmlstruct_thread_data *>(_data);
    const auto id = data->_id;
    WZipReaderSingle &reader = *data->_zip->get_reader(id);

    if(data->_page->load(reader, data->_ver_stroke) != OK)
        return (void *)1UL;

    return NULL;
}

static int xmlstruct_wait_for_thread(pthread_t *thread, int num)
{
    int i;
    void *ret_thread;
    int ret = 0;

    for(i = 0; i < num; i++){
        pthread_join(thread[i], &ret_thread);

        if(ret)
            ret = true;
    }

    return ret;
}

#define MANAGE_ERR()    \
    do{                 \
        return ERROR;   \
    }while(0)

int xmlstruct::loadbinario_4(class WZip &zip, int ver_stroke)
{
    size_t controll, newControll;
    int lenPage, counterPage;
    datastruct *data = _doc->datatouch;
    WZipReaderSingle reader(&zip, 0);

    if(!zip.openFileInZip(NAME_BIN))
        return ERROR;

    if(load_point_first_page(reader, *this->_doc->datatouch) < 0)
        MANAGE_ERR();

    if(read_zoom(reader, _doc->datatouch->_zoom) < 0)
        MANAGE_ERR();

    if(read_ctrl(reader, controll) < 0)
        MANAGE_ERR();

    if(read_number_page(reader, lenPage, *_doc->datatouch) < 0)
        MANAGE_ERR();

    size_t seek[lenPage];
    pthread_t thread[lenPage];
    struct xmlstruct_thread_data thread_data[lenPage];

    if(reader.read_by_size(seek, sizeof(size_t) * lenPage))
        MANAGE_ERR();

    WReadZip zipReader(&zip, lenPage, reader.get_offset(), seek);

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        data->newPage(n_style::white);

        thread_data[counterPage] = {
            ._zip           = &zipReader,
            ._id            = counterPage,
            ._page          = &_doc->datatouch->at_mod(counterPage),
            ._ver_stroke    = ver_stroke
        };

        pthread_create( &thread[counterPage],
                        NULL,
                        xmlstruct_thread_load,
                        &thread_data[counterPage]);
    }

    if(xmlstruct_wait_for_thread(thread, lenPage) < 0)
        MANAGE_ERR();

    zip.dealloc_file();

    newControll = _doc->createSingleControll();

    if(controll != newControll)
        return ERROR_CONTROLL;

    return OK;
}
