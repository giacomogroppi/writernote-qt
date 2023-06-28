#include "datawrite/savefile.h"
#include "stdlib.h"
#include "core/String/WString.h"
#include "datawrite/source_read_ext.h"
#include "images/fromimage.h"
#include "FileContainer/WZipWriterSingle.h"
#include "currenttitle/document.h"
#include "FileContainer/WZipWriterMulti.h"

static int savefile_save_seek(Document *doc, WZipWriterSingle &writer, size_t *seek)
{
    W_ASSERT(doc);
    const int lenPage = doc->lengthPage();
    writer.write_object(lenPage);

    static_assert(sizeof(*seek) == sizeof(size_t));

    writer.write(seek, sizeof(*seek) * lenPage);

    return 0;
}

struct savefile_thread_data{
    WZipWriterSingle *_writer;
    bool _saveImg;
    const Page *_page;
};

static void *salvafile_thread_save(void *_data)
{
    W_ASSERT(_data);
    auto *data = static_cast<struct savefile_thread_data *> (_data);
    const Page *_page = data->_page;
    const auto saveImg = data->_saveImg;
    WZipWriterSingle &writer = *data->_writer;

    const auto res = Page::write(writer, *_page);

    if(res != OK)
        return (void *)1UL;

    return nullptr;
}

static int savefile_wait_thread(pthread_t *thread, int num)
{
    int i;
    void *ret_thread;
    int ret = 0;

    for(i = 0; i < num; i++){
        pthread_join(thread[i], &ret_thread);

        if(ret_thread)
            ret = -1;
    }

    return ret;
}

static int savefile_save_multithread_start(Document *doc, WZipWriterSingle &writer, size_t *seek, cbool saveImg)
{
    int i;
    const auto l = doc->lengthPage();
    pthread_t thread[l];
    struct savefile_thread_data data_thread[l];
    WZipWriterMulti multi(nullptr, nullptr, l, seek, writer);

    if(multi.is_err())
        return -1;

    for(i = 0; i < l; i++){
        data_thread[i] = {
            ._writer    = multi.get_writer(i),
            ._saveImg   = saveImg,
            ._page      = &doc->at(i)
        };

        pthread_create(&thread[i], nullptr, salvafile_thread_save, &data_thread[i]);
    }

    if(savefile_wait_thread(thread, l) < 0)
        return -1;

#ifdef DEBUGINFO
    writer.set_offset(multi.get_last_offset());
#endif // DEBUGINFO

    return 0;
}

// TODO: remove this legacy method
int savefile::salvabinario(cbool saveImg)
{
    return ERROR;
}
