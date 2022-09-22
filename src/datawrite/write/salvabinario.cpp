#include "datawrite/savefile.h"
#include "stdlib.h"
#include <QMessageBox>
#include <QString>
#include "zip.h"
#include "datawrite/source_read_ext.h"
#include "images/fromimage.h"
#include "core/WZipWriterSingle.h"
#include "currenttitle/document.h"

static size_t savefile_get_size_binary(Document &doc)
{
    W_ASSERT(0);
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
    const auto sizeFile = savefile_get_size_binary(*_doc);

    writer.init(NULL, 0, sizeFile);

    /* first point */
    static_assert(sizeof(init) == sizeof(double) * 2);
    writer.write(init, sizeof(init));

    /* page len */
    writer.write_object(lenPage);

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        page = &_doc->datatouch->at(counterPage);

        err = page->save(writer, saveImg);
        if(err != OK)
            return ERROR;
    }

    writer.write_object(_doc->datatouch->getZoom());

    writer.write_object(controll);

    if(writer.commit_change(*_path, QByteArray(NAME_BIN)))
        return ERROR;

    return OK;
}

void savefile::removeFile(zip_t *file, const QString &name)
{

}
