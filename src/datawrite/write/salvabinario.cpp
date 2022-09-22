#include "datawrite/savefile.h"
#include "stdlib.h"
#include <QMessageBox>
#include <QString>
#include "zip.h"
#include "datawrite/source_read_ext.h"
#include "images/fromimage.h"
#include "core/WZipWriterSingle.h"
#include "currenttitle/document.h"
#include "core/WZipWriterMulti.h"

static size_t savefile_get_size_page(const page &_page, cbool saveImg)
{
    return _page.get_size_in_file(saveImg);
}

static size_t savefile_get_size_binary(Document &doc, cbool saveImg, size_t *seek)
{
    size_t size = 0;
    int i, len;

    len = doc.datatouch->lengthPage();

    {
        size += sizeof(int);            // number of page saved
        size += sizeof(size_t) * len;   // size seek array
    }

    for(i = 0; i < len; i++){
        const auto page = doc.datatouch->at(i);
        size += savefile_get_size_page(page, saveImg);

        seek[i] = size;
    }

    return 0;
}

static int savefile_save_seek(Document *doc, WZipWriterSingle &writer, size_t *seek)
{
    W_ASSERT(doc);
    const int lenPage = doc->datatouch->lengthPage();
    writer.write_object(lenPage);
    writer.write(seek, sizeof(*seek) * lenPage);

    return 0;
}

struct savefile_thread_data{
    WZipWriterMulti *_writer;
    int id;
    const page *_page;
};

static void *salvafile_thread_save(void *_data)
{
    W_ASSERT(_data);
    struct savefile_thread_data *data = static_cast<struct savefile_thread_data *> (_data);

    for(i = 0; i < l; i++){
        page = &_doc->datatouch->at(counterPage);

        err = page->save(writer, saveImg);
        if(err != OK)
            return ERROR;
    }

    return NULL;
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

static int savefile_save_multithread_start(Document *doc, WZipWriterSingle &writer, size_t *seek)
{
    int i;
    const auto l = doc->datatouch->lengthPage();
    pthread_t thread[l];
    struct savefile_thread_data data_thread[l];
    WZipWriterMulti multi(NULL, NULL, l, seek, writer);

    if(multi.is_err())
        return -1;

    for(i = 0; i < l; i++){
        data_thread[i] = {
            ._writer    = &multi,
            .id         = i,
            ._page      = &doc->datatouch->at(i)
        };

        pthread_create(&thread[i], NULL, salvafile_thread_save, &data_thread[i]);
    }

    if(savefile_wait_thread(thread, l) < 0)
        return -1;

    return 0;
}

int savefile::salvabinario(cbool saveImg)
{
    int counterPage, err = ERROR;
    const size_t controll = _doc->createSingleControll();
    const auto &pointInit = _doc->datatouch->getPointFirstPageNoZoom();
    const double init[2] = {pointInit.x() , pointInit.y() };
    cint lenPage = _doc->datatouch->lengthPage();
    const page *page;
    WZipWriterSingle writer;
    size_t seek[_doc->datatouch->lengthPage()];
    const auto sizeFile = savefile_get_size_binary(*_doc, saveImg, seek);

    writer.init(NULL, 0, sizeFile);

    /* first point */
    static_assert(sizeof(init) == sizeof(double) * 2);
    writer.write(init, sizeof(init));

    /* page len */
    writer.write_object(lenPage);

    savefile_save_seek(this->_doc, writer, seek);

    savefile_save_multithread_start(_doc, writer, this->_path->constData(), NAME_BIN, seek);

    writer.write_object(_doc->datatouch->getZoom());

    writer.write_object(controll);

    if(writer.commit_change(*_path, QByteArray(NAME_BIN)))
        return ERROR;

    return OK;
}

void savefile::removeFile(zip_t *file, const QString &name)
{

}
