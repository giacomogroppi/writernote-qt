#include "datawrite/savefile.h"
#include "stdlib.h"
#include <QMessageBox>
#include <QString>
#include "zip.h"
#include "datawrite/source_read_ext.h"
#include "images/fromimage.h"
#include "currenttitle/document.h"

static int freezip(zip_source_t *files){
    zip_source_free(files);
    return ERROR;
}

int savefile::salvabinario(zip_t *filezip)
{
    int len, counterPage, err = ERROR;
    size_t controll;
    zip_source_t *file;
    zip_error_t errore;
    const auto &pointInit = currenttitle->datatouch->getPointFirstPageNoZoom();
    const double init[2] = {pointInit.x() , pointInit.y() };
    cint lenPage = currenttitle->datatouch->lengthPage();
    const page *page;
    const auto zoom = currenttitle->datatouch->getZoom();

    file = zip_source_buffer_create(0, 0, 0, &errore);

    zip_source_begin_write(file);

    /* first point */
    WRITE_ON_SIZE(file, init, sizeof(double)*2);

    /* page len */
    WRITE_ON_SIZE(file, &lenPage, sizeof(lenPage));
    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        page = &currenttitle->datatouch->at(counterPage);
        len = page->lengthStroke();

        /* stroke len */
        WRITE_ON_SIZE(file, &len, sizeof(len));

        err = page->save(file);
        if(err != OK)
            goto error;
    }

    WRITE_ON_SIZE(file, &zoom, sizeof(zoom));

    controll = currenttitle->createSingleControll();
    WRITE_ON_SIZE(file, &controll, sizeof(size_t));

    if(!savefile::commitChange(file))
        return freezip(file);
    if(!savefile::addFile(filezip, NAME_BIN, file))
        return freezip(file);

    return OK;

    error:
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
