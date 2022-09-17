#include "datawrite/savefile.h"
#include "stdlib.h"
#include <QMessageBox>
#include <QString>
#include "zip.h"
#include "datawrite/source_read_ext.h"
#include "images/fromimage.h"
#include "core/WZipWriterSingle.h"
#include "currenttitle/document.h"

static int savefile::save_zoom(WZipWriterSingle &writer, )

int savefile::salvabinario(WZipWriter &filezip, cbool saveImg)
{
    int counterPage, err = ERROR;
    const size_t controll = _doc->createSingleControll();
    const auto &pointInit = _doc->datatouch->getPointFirstPageNoZoom();
    const double init[2] = {pointInit.x() , pointInit.y() };
    cint lenPage = _doc->datatouch->lengthPage();
    const page *page;
    const auto zoom = _doc->datatouch->getZoom();

    file = zip_source_buffer_create(0, 0, 0, &errore);

    zip_source_begin_write(file);

    /* first point */
    SOURCE_WRITE(file, init, sizeof(double) * 2);

    /* page len */
    SOURCE_WRITE_GOTO_SIZE(file, &lenPage, sizeof(lenPage));

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        page = &_doc->datatouch->at(counterPage);

        err = page->save(file, saveImg);
        if(err != OK)
            goto delete_;
    }

    SOURCE_WRITE_GOTO_SIZE(file, &zoom, sizeof(zoom));

    SOURCE_WRITE_GOTO_SIZE(file, &controll, sizeof(controll));

    if(!savefile::commitChange(file))
        return freezip(file);
    if(!savefile::addFile(filezip, NAME_BIN, file))
        return freezip(file);

    return OK;

delete_:
    zip_source_free(file);
    return err;
}

void savefile::removeFile(zip_t *file, const QString &name)
{
    struct zip_stat stat;
    zip_stat_init(&stat);
    zip_stat(file, name.toUtf8().constData(), 0, &stat);
    zip_delete(file, stat.index);
}
