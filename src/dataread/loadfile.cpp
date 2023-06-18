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
    if (file.readRaw(data, size) != size)
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

bool xmlstruct::manageMessage(const int res, const std::function<void (const WString &)>& showMessage)
{
    switch (res) {
        case OK: return true;
        case ERROR: {
            showMessage("We had an internal problem loading the file");
            return false;
        }
        case ERROR_VERSION:{
            showMessage("This file is too old to be read");
            return false;
        }
        case ERROR_VERSION_NEW:{
            showMessage("This file was created with a later version of writernote, and I cannot read the file.");
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
    int err = 0;
    int tmp_ver;
    bool ok;

    static_assert(sizeof(tmp_ver) == xmlstruct::get_offset_start());

    _doc->reset();
    auto zip = WZip(*_path, ok);
    if(!ok)
        return ERROR;

    if(!zip.openFileInZip(NAME_FILE))
        return ERROR;

    {
        WZipReaderSingle reader(&zip, 0);
        if(reader.readObject(tmp_ver) < 0)
            return ERROR;
    }

    static_assert(CURRENT_VERSION_CURRENT_TITLE == 9);

    switch (tmp_ver) {
    case 0 ... CURRENT_VERSION_CURRENT_TITLE - 1:
        err = xmlstruct_read_file_old(tmp_ver, zip, LoadPdf, LoadImg);
        break;

    case CURRENT_VERSION_CURRENT_TITLE:
        err = load_file_9(_doc, zip, LoadPdf, LoadImg);
        break;
    default:
        goto error_new_version;
    }

    if(err != OK)
        goto free_;

    this->_doc->triggerNewView(-1, true);

    _doc->triggerVisibility(Page::getHeight() * _doc->lengthPage());
    return OK;

    free_:
    return err;

    /*
     * in case we can not operate with the file because it's too old
    */

    error_version:
    return ERROR_VERSION;

    error_new_version:
    return ERROR_VERSION_NEW;
}

/**
 * the function automatically opens and
 * closes the file containing the audio
 * TODO -> adjust the function so that more
 * than one audio file can be read, and add
 * the currenttitle data to support more
 * than one audio
 *
 * TODO -> load audio into the buffer only if you are really opening a new copybook
*/
#define CLOSE_ZIP_AUDIO(x, y) zip_fclose(x); \
    zip_close(y);

int load_audio(WByteArray &array, const WString &path)
{
    int error;
    bool ok;
    WZip zip(path.toUtf8(), ok);

    array.clear();

    if(!ok)
        return ERROR;

    error = xmlstruct::readFile(zip.get_zip(), array, true, NAME_AUDIO, false);

    return error;
}

int xmlstruct::load_file_9(Document *doc, WZip &zip, cbool LoadPdf, cbool LoadImg)
{
    unsigned len_pdf, len_img;
    int ver_stroke;
    unsigned char controllo_parita = 0;
    ImageContainerDrawable::load_res_img res_img;
    WZipReaderSingle singleReader(&zip, xmlstruct::get_offset_start());
    WString audioPath;

    if(singleReader.readObject(ver_stroke))
        return ERROR;

    {
        int tmp;
        static_assert(sizeof(tmp) == sizeof(int));
        if(singleReader.readObject(tmp))
            return ERROR;
        doc->setRecordStatus(static_cast<Document::AudioRecordStatus>(tmp));
    }

    if (singleReader.readString(audioPath))
        return ERROR;

    doc->setAudioPath(audioPath);

    if (singleReader.readObject(len_pdf) || singleReader.readObject(len_img))
        return ERROR;

    if (LoadImg) {
        res_img = doc->loadImage(singleReader, len_img);
        if(res_img != ImageContainerDrawable::load_res_img::ok){
            return ERROR;
        }
    }

#ifdef PDFSUPPORT
    if(LoadPdf){
        const auto res = doc->load_pdf(singleReader, static_cast<int>(len_pdf), *doc);

        if(res != frompdf::ok)
            return ERROR;
    }
#endif // PDFSUPPORT

    zip.dealloc_file();

    const auto res = loadbinario_4(zip, ver_stroke);
    if(res == ERROR)
        return res;
    else if(res == ERROR_CONTROLL)
        /* we want to continue to load the file, but we need to return we had a problem */
        controllo_parita = 1;

    if(controllo_parita)
        return ERROR_CONTROLL;

    return OK;
}
