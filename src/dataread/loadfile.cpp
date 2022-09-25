#include "frompdf/frompdf.h"
#include "images/fromimage.h"
#include "xmlstruct.h"
#include "datawrite/source_read_ext.h"
#include "utils/common_error_definition.h"
#include "utils/areyousure/areyousure.h"
#include "testing/memtest.h"
#include "core/WZip.h"
#include "core/WZipReaderSingle.h"

int xmlstruct::load_stringa(zip_file_t *f, QString &stringa)
{
    int tmp;
    char *str;

    SOURCE_READ_RETURN_SIZE(f, &tmp, sizeof(tmp));

    if(tmp){
        str = (char *)WMalloc(sizeof(char) * (tmp + 1));

        SOURCE_READ_RETURN_SIZE(f, str, sizeof(char)*tmp);

        str[tmp] = '\0';

        stringa = str;

        WFree(str);
    }

    return OK;
}

int xmlstruct::load_stringa(zip_file_t *f, QByteArray &str)
{
    QString tmp;
    int flag;

    flag = xmlstruct::load_stringa(f, tmp);
    
    str = tmp.toUtf8();

    return flag;
}

int xmlstruct::readFile(zip_t *fileZip, QByteArray &arr,
                        cbool clear, const QString &path,
                        cbool closeZip)
{
    zip_file_t *file;
    void *data;

    const size_t size = xmlstruct::sizeFile(fileZip, path);

    if(clear)
        arr.clear();

    if(unlikely(!size)){
        const auto prio = (path.indexOf("audio") != -1) ? log_ui::possible_bug : log_ui::info;
        log_write->write(QString("File %1 has 0 byte").arg(path), prio);
        return OK;
    }

    file = zip_fopen(fileZip, path.toUtf8().constData(), 0);
    if(file == nullptr)
        return ERROR;

    data = WMalloc(size);
    SOURCE_READ_GOTO(file, data, size);

    arr.append((cchar *)data, size);

    WFree(data);

    zip_fclose(file);
    if(closeZip)
        zip_close(fileZip);

    return OK;

    free_:
    zip_fclose(file);
    if(closeZip)
        zip_close(fileZip);
    return ERROR;
}

bool xmlstruct::manageMessage(const int res)
{
    switch (res) {
        case OK: return true;
        case ERROR: {
            dialog_critic("We had an internal problem loading the file");
            return false;
        }
        case ERROR_VERSION:{
            dialog_critic("This file is too old to be read");
            return false;
        }
        case ERROR_VERSION_NEW:{
            dialog_critic("This file was created with a later version of writernote, and I cannot read the file.");
            return false;
        }
        case ERROR_CONTROLL:{
            return areyousure("Corrupt file", "The file is corrupt, do you want to open it anyway?");
        }
        case ERROR_MULTIPLE_COPYBOOK:{
            user_message("Writernote has decided to completely change the file format, now it will no longer be possible to have more than one copybook in a file. \nTo use files created with writernote versions lower than or equal to 1.5.2h you must:\nOpen the file with a compressed archive manager, extract all the files that have the same prefix, i.e. the copybook name, and move them into a new writernote file, changing all the names of the same copybook with the prefix data.");
            return false;
        }
        case ERROR_VERSION_KEY:{
            user_message("This file was written with the pen, and it is not possible to light it.");
            return false;
        }
        default:
            break;
    }
    assert(0);
    return false;
}

int xmlstruct::load_multiplestring(zip_file_t *f, QList<QString> &lista, QList<int> &data)
{
    int i, lunghezza, temp;

    SOURCE_READ_RETURN_SIZE(f, &lunghezza, sizeof(lunghezza));
    if(!lunghezza)
        return OK;


    QString temp_;

    for(i = 0; i < lunghezza; i++){
        LOAD_STRINGA_RETURN(f, temp_);

        lista.append(temp_);
    }

    for(i = 0; i < lunghezza; i++){
        SOURCE_READ_RETURN_SIZE(f, &temp, sizeof(temp));

        data.append(temp);
    }

    return OK;
}

uchar xmlstruct::controllOldVersion(zip_t *file)
{
    zip_file_t *tt;

    tt = zip_fopen(file, "indice.xml", 0);
    if(tt == nullptr){
        return 0;
    }
    zip_fclose(tt);
    zip_close(file);
    return 1;
}

#ifdef ALL_VERSION
int xmlstruct::xmlstruct_read_file_old(int ver, WZip &zip, cbool LoadPdf, cbool LoadImg)
{
    int err;
    W_ASSERT(ver >= 0);
    W_ASSERT(ver <= 8);

    WZipReaderSingle reader(&zip, xmlstruct::get_offset_start());

    switch (ver) {
        case 0 ... 2:
            err = load_file_2(reader);
            break;
        case 3:
            err = load_file_3(reader);
            break;
        case 4:
            err = load_file_4(reader);
            break;
        case 5:
            err = load_file_5(reader, LoadPdf, LoadImg);
            break;
        case 6:
            err = load_file_6(reader, LoadPdf, LoadImg);
            break;
        case 7:
            err = load_file_7(reader, LoadPdf, LoadImg);
            break;
        case 8:
            err = load_file_8(reader, LoadPdf, LoadImg);
            break;
        default:
            std::abort();
    }

    return err;
}
#endif

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

    if(xmlstruct::controllOldVersion(zip.get_zip()))
        return ERROR_MULTIPLE_COPYBOOK;

    if(!zip.openFileInZip(NAME_FILE))
        return ERROR;

    {
        WZipReaderSingle reader(&zip, 0);
        if(reader.read_object(tmp_ver) < 0)
            return ERROR;
    }

    static_assert(CURRENT_VERSION_CURRENT_TITLE == 9);

    switch (tmp_ver) {
#ifdef ALL_VERSION
    case 0 ... CURRENT_VERSION_CURRENT_TITLE - 1:
        err = xmlstruct_read_file_old(tmp_ver, zip, LoadPdf, LoadImg);
        break;
#else
    case 0 ... (CURRENT_VERSION_CURRENT_TITLE - 1):
        goto error_version;
#endif
    case CURRENT_VERSION_CURRENT_TITLE:
        err = load_file_9(_doc, zip, LoadPdf, LoadImg);
        break;
    default:
        goto error_new_version;
    }

    if(err != OK)
        goto free_;

    /*
     * after we load all the file we
     * set the version of the new
     * in this way, even the files created
     * by previous versions of writernote
     * will be updated to the new versions
    */
    this->_doc->versione = CURRENT_VERSION_CURRENT_TITLE;

    this->_doc->datatouch->triggerNewView(-1, true);

    _doc->datatouch->triggerVisibility(page::getHeight() * _doc->datatouch->lengthPage());
    return OK;

    free_:
    return err;

    /*
     * in case we can not operate with the file because it's too old
    */
#ifndef ALL_VERSION
    error_version:
    return ERROR_VERSION;
#endif
    error_new_version:
    return ERROR_VERSION_NEW;
}

/*
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

int load_audio(QByteArray &array, const QString &path)
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

size_t  xmlstruct::sizeFile(zip_t *filezip, const char *namefile)
{
    struct zip_stat st;
    zip_stat_init(&st);

    /*
     * Upon successful completion 0 is returned. Otherwise,
     * -1 is returned and the error information in archive
     * is set to indicate the error
    */
    if(zip_stat(filezip, namefile, ZIP_STAT_SIZE, &st) < 0)
        return 0;

    return st.size;
}

int xmlstruct::load_file_9(Document *doc, WZip &zip, cbool LoadPdf, cbool LoadImg)
{
    int ver_stroke;
    uchar controllo_parita = 0;
    fromimage::load_res res_img;
    WZipReaderSingle singleReader(&zip, xmlstruct::get_offset_start());

    static_assert(sizeof(doc->count_img) == sizeof(doc->count_pdf));
    static_assert(sizeof(doc->count_img) == 4);

    if(singleReader.read_object(ver_stroke))
        return ERROR;

    {
        int tmp;
        static_assert(sizeof(tmp) == sizeof(int));
        if(singleReader.read_object(tmp))
            return ERROR;
        doc->se_registato = static_cast<Document::n_audio_record>(tmp);
    }

    if(singleReader.read_string(doc->audio_position_path))
        return ERROR;

    if(singleReader.read_object(doc->count_pdf) || singleReader.read_object(doc->count_img))
        return ERROR;

    if(LoadImg){
        res_img = doc->m_img->load(singleReader);
        if(res_img != fromimage::load_res::ok){
            return ERROR;
        }
    }

#ifdef PDFSUPPORT
    if(LoadPdf){
        frompdf::load_res res = doc->m_pdf->load(singleReader, nullptr);

        if(res != frompdf::ok)
            return ERROR;
    }
#endif

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
