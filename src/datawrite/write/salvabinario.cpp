#include "datawrite/savefile.h"
#include "stdlib.h"
#include "core/String/WString.h"
#include "datawrite/source_read_ext.h"
#include "images/fromimage.h"
#include "FileContainer/WZipWriterSingle.h"
#include "currenttitle/document.h"
#include "FileContainer/WZipWriterMulti.h"

static size_t savefile_get_size_page(const Page &_page, cbool saveImg)
{
    const auto res = _page.get_size_in_file(saveImg);
    return res;
}

static size_t savefile_get_size_binary(Document &doc, cbool saveImg, size_t *seek)
{
    size_t size = 0;
    int i, len;

    size += sizeof(double) * 2;         // first point

    size += sizeof(doc.getZoom());   // zoom
    size += sizeof(size_t);                     // controll

    {
        len = doc.lengthPage();
        size += sizeof(int);            // number of page saved
        size += sizeof(size_t) * len;   // size seek array
    }

    for(i = 0; i < len; i++){
        const auto &page = doc.at(i);
        seek[i] = size;
        size += savefile_get_size_page(page, saveImg);
    }

    return size;
}

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

int savefile::salvabinario(cbool saveImg)
{
    const size_t controll = _doc->createSingleControll();
    const auto &pointInit = _doc->getPointFirstPageNoZoom();
    const double init[2] = { pointInit.x(), pointInit.y() };
    WZipWriterSingle writer;
    size_t seek[_doc->lengthPage()];
    const auto sizeFile = savefile_get_size_binary(*_doc, saveImg, seek);

    writer.init(nullptr, 0, sizeFile);
    WDebug(true, "Size: " << sizeFile);

    /* first point */
    static_assert(sizeof(init) == sizeof(double) * 2);
    writer.write(init, sizeof(init));

    writer.write_object(_doc->getZoom());

    writer.write_object(controll);

    savefile_save_seek(this->_doc, writer, seek);

#ifdef DEBUGINFO
    if(_doc->lengthPage())
        W_ASSERT(writer.get_offset() == seek[0]);
#endif // DEBUGINFO

    if(savefile_save_multithread_start(_doc, writer, seek, saveImg) < 0)
        return ERROR;

    if(writer.commit_change(*_path, WByteArray(NAME_BIN)) < 0)
        return ERROR;

    return OK;
}
