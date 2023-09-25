#include "frompdf/frompdf.h"
#include "images/fromimage.h"
#include "xmlstruct.h"
#include "datawrite/source_read_ext.h"
#include "utils/common_error_definition.h"
#include "testing/memtest.h"
#include "FileContainer/WZip.h"
#include "FileContainer/WZipReaderSingle.h"
#include "log/log_ui/log_ui.h"
#include <cassert>

int xmlstruct::readFile(FileContainer *fileZip, WByteArray &arr,
                        cbool clear, const WString &path,
                        cbool closeZip)
{
    void *data;

    const size_t size = fileZip->sizeOfFile(path);

    if(clear)
        arr.clear();

    if(un(!size)){
        const auto prio = (path.indexOf("audio") != -1) ? log_ui::possible_bug : log_ui::info;
        log_write->write(WString("File %1 has 0 byte").arg(path), prio);
        return OK;
    }

    FileReader file = fileZip->getFileReader(path);
    if(!file.isOk())
        return ERROR;

    data = WMalloc(size);
    if (file.read(data, size) < 0)
        goto free_;

    arr.append((const char *)data, size);

    WFree(data);
    fileZip->closeFileReader(file);
    if(closeZip)
        fileZip->close();

    return OK;

    free_:
    fileZip->closeFileReader(file);
    if(closeZip)
        fileZip->close();
    return ERROR;
}

bool xmlstruct::manageMessage(const int res, const Fn<void (const WString &)>& showMessage)
{
    switch (res) {
        case OK: return true;
        case ERROR: {
            showMessage("We had an internal problem loading the file");
            return false;
        }
        case ERROR_VERSION:{
            showMessage("This file is too old to be load");
            return false;
        }
        case ERROR_VERSION_NEW:{
            showMessage("This file was created with a later version of writernote, and I cannot load the file.");
            return false;
        }
        case ERROR_CONTROLL:{
            return true;
        }
        case ERROR_MULTIPLE_COPYBOOK:{
            showMessage("Writernote has decided to completely change the file format, now it will no longer be possible to have more than one copybook in a file. \nTo use files created with writernote versions lower than or equal to 1.5.2h you must:\nOpen the file with a compressed archive manager, extract all the files that have the same prefix, i.e. the copybook name, and move them into a new writernote file, changing all the names of the same copybook with the prefix data.");
            return false;
        }
        case ERROR_VERSION_KEY:{
            showMessage("This file was written with the pen, and it is not possible to light it.");
            return false;
        }
        default:
            break;
    }
    assert(0);
    return false;
}

int xmlstruct::xmlstruct_read_file_old(int ver, WZip &zip, cbool LoadPdf, cbool LoadImg)
{
    int err;
    W_ASSERT(ver >= 0);
    W_ASSERT(ver <= 8);

    WZipReaderSingle reader(&zip, xmlstruct::get_offset_start());

    switch (ver) {

        default:
            std::abort();
    }

    return err;
}

int xmlstruct::loadfile(cbool LoadPdf, cbool LoadImg)
{
    FileContainer fileContainer(*_path);

    if (!fileContainer.isOk())
        return ERROR;

    auto fileReader = fileContainer.getFileReader(xmlstruct::nameFile);

    auto [res, versionController] = VersionFileController::load(fileReader);

    if (res < 0)
        return ERROR;

    auto [r, doc] = Document::load(versionController, fileReader);

    if (r < 0)
        return ERROR;

    *_doc = std::move(doc);

    return OK;
}

/**
 * the function automatically opens and
 * closes the file containing the audio
 * TODO -> adjust the function so that more
 * than one audio file can be load, and add
 * the currenttitle data to support more
 * than one audio
 *
 * TODO -> load audio into the buffer only if you are really opening a new copybook
*/
