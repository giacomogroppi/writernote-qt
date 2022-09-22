#include "datawrite/savefile.h"
#include "stdlib.h"
#include <QMessageBox>
#include <QString>
#include "zip.h"
#include "datawrite/source_read_ext.h"
#include "images/fromimage.h"
#include "core/WZipWriterSingle.h"
#include "currenttitle/document.h"

static size_t savefile_get_size_page(const page &_page, cbool saveImg)
{
    return _page.get_size_in_file(saveImg);
}

static size_t savefile_get_size_binary(Document &doc, cbool saveImg)
{
    size_t size = 0;
    int i, len;

    len = doc.datatouch->lengthPage();

    for(i = 0; i < len; i++){
        const auto page = doc.datatouch->at(i);
        size += savefile_get_size_page(page, saveImg);
    }

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
    const auto sizeFile = savefile_get_size_binary(*_doc, saveImg);

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
