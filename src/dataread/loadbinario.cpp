#include "xmlstruct.h"
#include "currenttitle/document.h"
#include "utils/WCommonScript.h"
#include "core/WReadZip.h"
#include "FileContainer/WZipReaderSingle.h"
#include <pthread.h>
#include "utils/common_error_definition.h"
#include "sheet/style_struct.h"

static int load_point_first_page(WZipReaderSingle &zip, DataStruct &doc)
{
    double init[2];
    /* point first page */

    static_assert(sizeof(init) == sizeof(double) * 2);

    if(zip.readBySize(init, sizeof(init)) < 0)
        return -1;

    doc.setPointFirstPage(PointF(init[0], init[1]));

    return 0;
}

static int read_number_page(WZipReaderSingle &zip, int &len)
{
    /* page len */
    static_assert(sizeof(len) == sizeof(int));

    if(zip.readObject(len) < 0)
        return -1;

    W_ASSERT(len > 0);

    return 0;
}

static int read_zoom(WZipReaderSingle &zip, double &zoom)
{
    static_assert(sizeof(zoom) == sizeof(double));

    if(zip.readObject(zoom) < 0)
        return -1;

    return 0;
}

static int read_ctrl(WZipReaderSingle &zip, size_t &ctrl)
{
    static_assert(sizeof(ctrl) == sizeof(size_t));
    if(zip.readObject(ctrl) < 0)
        return -1;
    return 0;
}

struct xmlstruct_thread_data{
    WZipReaderSingle        *_reader;
    Page                    *_page;
    VersionFileController   _versionController;
};

static void *xmlstruct_thread_load(void *_data)
{
    auto *data = static_cast<struct xmlstruct_thread_data *>(_data);
    WZipReaderSingle &reader = *data->_reader;
    auto [res, page] = Page::load(data->_versionController, *data->_reader);

    if (res)
        return (void *)1UL;

    *data->_page = std::move(page);

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

// TODO: move this method in DataStruct to make the load multithread
static int xmlstruct_create_thread(WZip& zip, int lenPage, size_t *seek, DataStruct *data)
{
    int i;
    pthread_t thread[lenPage];
    struct xmlstruct_thread_data thread_data[lenPage];
    WReadZip zipReader(&zip, lenPage, seek);

    for (i = 0; i < lenPage; i++) {
        data->newPage(n_style::white);
    }

    for (i = 0; i < lenPage; i ++) {
        thread_data[i] = {
                ._reader            = zipReader.get_reader(i),
                ._page              = &data->at_mod(i),
                ._versionController = VersionFileController()
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
