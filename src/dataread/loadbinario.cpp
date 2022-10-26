#include "xmlstruct.h"
#include "currenttitle/document.h"
#include <zip.h>
#include "sheet/fast-sheet/fast_sheet_ui.h"
#include "utils/WCommonScript.h"
#include "core/WReadZip.h"
#include "core/WZipReaderSingle.h"
#include <pthread.h>
#include "utils/common_error_definition.h"

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

static int read_number_page(WZipReaderSingle &zip, int &len)
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
    WZipReaderSingle    *_reader;
    Page                *_page;
    int                 _ver_stroke;
};

static void *xmlstruct_thread_load(void *_data)
{
    auto *data = static_cast<struct xmlstruct_thread_data *>(_data);
    WZipReaderSingle &reader = *data->_reader;
    const auto res = data->_page->load(reader, data->_ver_stroke);
    if(res != OK)
        return (void *)1UL;

    return nullptr;
}

static int xmlstruct_wait_for_thread(pthread_t *thread, int num)
{
    int i;
    void *ret_thread;
    int ret = 0;

    for(i = 0; i < num; i++){
        pthread_join(thread[i], &ret_thread);

        if(ret_thread)
            ret = true;
    }

    return ret;
}

#define MANAGE_ERR()    \
    do{                 \
        return ERROR;   \
    }while(0)

static int xmlstruct_create_thread(WZip& zip, int lenPage, size_t *seek, datastruct *data, int ver_stroke)
{
    int i;
    pthread_t thread[lenPage];
    struct xmlstruct_thread_data thread_data[lenPage];
    WReadZip zipReader(&zip, lenPage, seek);

    for(i = 0; i < lenPage; i++){
        data->newPage(n_style::white);
    }

    for(i = 0; i < lenPage; i ++){
        thread_data[i] = {
                ._reader        = zipReader.get_reader(i),
                ._page          = &data->at_mod(i),
                ._ver_stroke    = ver_stroke
        };

        pthread_create( &thread[i],
                        nullptr,
                        xmlstruct_thread_load,
                        &thread_data[i]);
    }

    if(xmlstruct_wait_for_thread(thread, lenPage) < 0){
        return -1;
    }

    return 0;
}

int xmlstruct::loadbinario_4(class WZip &zip, int ver_stroke)
{
    size_t controll, newControll;
    int lenPage;
    WZipReaderSingle reader(&zip, 0);

    if(!zip.openFileInZip(NAME_BIN))
        return ERROR;

    if(load_point_first_page(reader, *this->_doc) < 0)
        MANAGE_ERR();

    if(read_zoom(reader, _doc->_zoom) < 0)
        MANAGE_ERR();

    if(read_ctrl(reader, controll) < 0)
        MANAGE_ERR();

    if(read_number_page(reader, lenPage) < 0)
        MANAGE_ERR();

    size_t seek[lenPage];

    if(reader.read_by_size(seek, sizeof(size_t) * lenPage))
        MANAGE_ERR();

    if(xmlstruct_create_thread(zip, lenPage, seek, _doc, ver_stroke) < 0)
        MANAGE_ERR();

    zip.dealloc_file();

    newControll = _doc->createSingleControll();

    if(controll != newControll)
        return ERROR_CONTROLL;

    return OK;
}
