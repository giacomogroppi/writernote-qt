#include "../savefile.h"
#include "stdlib.h"
#include <QMessageBox>
#include <QString>
#include "zip.h"
#include "../source_read_ext.h"
#include "../../images/fromimage.h"
#include "../../currenttitle/document.h"

static int freezip(zip_source_t *files){
    zip_source_free(files);
    return ERROR;
}

#if 0
int savefile::salvabinario(zip_t *filezip)
{
    int i, len, counterPage;
    size_t controll;
    zip_source_t *file;
    zip_error_t errore;
    const auto &pointInit = currenttitle->datatouch->getPointFirstPageNoZoom();
    const double init[2] = {pointInit.x() , pointInit.y() };
    const int lenPage = currenttitle->datatouch->lengthPage();
    const page *page;
    const point_s *point;
    const double zoom = currenttitle->datatouch->getZoom();
    file = zip_source_buffer_create(0, 0, 0, &errore);

    zip_source_begin_write(file);

    /* first point */
    WRITE_ON_SIZE(file, init, sizeof(double)*2);

    /* page len */
    WRITE_ON_SIZE(file, &lenPage, sizeof(lenPage));
    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        page = currenttitle->datatouch->at(counterPage);

        len = page->length();
        WRITE_ON_SIZE(file, &len, sizeof(len));

        for(i = 0; i < len; i++){
            point = page->at(i);
            WRITE_ON_SIZE(file, point, sizeof(*point));
        }
    }

    WRITE_ON_SIZE(file, &zoom, sizeof(double));

    controll = currenttitle->createSingleControll();
    WRITE_ON_SIZE(file, &controll, sizeof(size_t));

    if(zip_source_commit_write(file) == -1)
        return freezip(file);

    if(zip_file_add(filezip,
                 NAME_BIN,
                 file,
                 ZIP_FL_OVERWRITE) == -1)
        return freezip(file);

    return OK;

    error:
    zip_source_free(file);
    return ERROR;

}
#endif

int savefile::salvabinario(zip_t *filezip)
{
    int i, len, counterPage;
    size_t controll;
    zip_source_t *file;
    zip_error_t errore;
    const auto &pointInit = currenttitle->datatouch->getPointFirstPageNoZoom();
    const double init[2] = {pointInit.x() , pointInit.y() };
    const int lenPage = currenttitle->datatouch->lengthPage();
    const page *page;
    const double zoom = currenttitle->datatouch->getZoom();
    file = zip_source_buffer_create(0, 0, 0, &errore);

    zip_source_begin_write(file);

    /* first point */
    WRITE_ON_SIZE(file, init, sizeof(double)*2);

    /* page len */
    WRITE_ON_SIZE(file, &lenPage, sizeof(lenPage));
    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        page = &currenttitle->datatouch->at(counterPage);

        len = page->lengthStroke();
        WRITE_ON_SIZE(file, &len, sizeof(len));

        for(i = 0; i < len; i++){
            const stroke &stroke = page->atStroke(i);
            if(stroke.save(file) != OK)
                goto error;
        }
    }

    WRITE_ON_SIZE(file, &zoom, sizeof(double));

    controll = currenttitle->createSingleControll();
    WRITE_ON_SIZE(file, &controll, sizeof(size_t));

    if(zip_source_commit_write(file) == -1)
        return freezip(file);

    if(zip_file_add(filezip,
                 NAME_BIN,
                 file,
                 ZIP_FL_OVERWRITE) == -1)
        return freezip(file);

    return OK;

    error:
    zip_source_free(file);
    return ERROR;
}

void savefile::removeFile(zip_t *file, const QString &name)
{
    struct zip_stat stat;
    zip_stat_init(&stat);
    zip_stat(file, name.toUtf8().constData(), 0, &stat);
    zip_delete(file, stat.index);
}
