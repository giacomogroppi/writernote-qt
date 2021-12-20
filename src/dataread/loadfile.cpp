#include "xmlstruct.h"
#include "images/fromimage.h"
#include "datawrite/source_read_ext.h"
#include "utils/common_error_definition.h"
#include "frompdf/frompdf.h"
#include "utils/areyousure/areyousure.h"

int xmlstruct::load_stringa(zip_file_t *f, QString &stringa){
    int tmp;
    char *str;

    SOURCE_READ_RETURN(f, &tmp, sizeof(int));

    if(tmp){
        str = (char *)malloc(sizeof(char) * (tmp + 1));

        SOURCE_READ_RETURN(f, str, sizeof(char)*tmp);

        str[tmp] = '\0';

        stringa = str;

        free(str);
    }

    return OK;
}

int xmlstruct::readFile(zip_t *fileZip, QByteArray &arr,
                        const bool clear, const QString &path,
                        const bool closeZip)
{
    if(clear)
        arr.clear();

    size_t size;
    zip_file_t *file;
    uchar __r;

    size = xmlstruct::sizeFile(fileZip, path);

    file = zip_fopen(fileZip, path.toUtf8().constData(), 0);
    if(file == NULL)
        return ERROR;

    while(size){
        SOURCE_READ_GOTO(file, &__r, sizeof(uchar));

        arr.append(__r);
        size --;
    }

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
    }
    assert(0);
    return false;
}

int xmlstruct::load_multiplestring(zip_file_t *f, QList<QString> &lista, QList<int> &data){
    int i, lunghezza, temp;

    SOURCE_READ_RETURN(f, &lunghezza, sizeof(int));
    if(!lunghezza)
        return OK;


    QString temp_;

    for(i=0; i<lunghezza; i++){
        LOAD_STRINGA_RETURN(f, temp_);

        lista.append(temp_);
    }

    for(i=0; i<lunghezza; i++){
        SOURCE_READ_RETURN(f, &temp, sizeof(int));

        data.append(temp);
    }

    return OK;
}

uchar xmlstruct::controllOldVersion(zip_t *file)
{
    zip_file_t *tt;

    tt = zip_fopen(file, "indice.xml", 0);
    if(tt == NULL){
        return 0;
    }
    zip_fclose(tt);
    zip_close(file);
    return 1;
}

#define CLOSE_ZIP(x, y) zip_fclose(x);zip_close(y);
#define LOAD_IMAGE(x,y) if(load_image(x, y) != OK)goto free_;

#define LOAD_BINARIO(x) if(loadbinario(x) == ERROR) goto free_;
#define LOAD_BINARIO_RETURN(x, function) if(function(x) == ERROR) return ERROR;

int xmlstruct::loadfile(const bool LoadPdf, const bool LoadImg){
    int err = 0;
    zip_t *filezip;
    zip_file_t *f;
    int tmp_ver;

    currenttitle->reset();

    filezip = zip_open(path_->toUtf8().constData(),
                       ZIP_CREATE,
                       &err);

    if (filezip == NULL)
        return ERROR;

    if(xmlstruct::controllOldVersion(filezip))
        return ERROR_MULTIPLE_COPYBOOK;

    f = zip_fopen(filezip, NAME_FILE, 0);
    if(f == NULL){
        zip_close(filezip);
        return ERROR;
    }

    SOURCE_READ_GOTO(f, &tmp_ver, sizeof(tmp_ver));

    if(tmp_ver <= 2){
#ifdef ALL_VERSION
        err = load_file_2(currenttitle, f, filezip);
    }else if(tmp_ver == 3){
        err = load_file_3(currenttitle, f, filezip);
    }else if(tmp_ver == 4){
        err = load_file_4(currenttitle, f, filezip);
    }else if(tmp_ver == 5){
        err = load_file_5(currenttitle, f, filezip, LoadPdf, LoadImg);
    }else if(tmp_ver == 6){
        err = load_file_6(currenttitle, f, filezip, LoadPdf, LoadImg);
    }else if(tmp_ver == 7){
        err = load_file_7(currenttitle, f, filezip, LoadPdf, LoadImg);
    }
#else
        goto error_version;
    }
#endif
    else if(tmp_ver > 8)
        goto error_new_version;

    if(tmp_ver == 8){
        err = load_file_8(currenttitle, f, filezip, LoadPdf, LoadImg);
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
    currenttitle->versione = CURRENT_VERSION_CURRENT_TITLE;

    this->currenttitle->datatouch->triggerNewView(-1, true);
    CLOSE_ZIP(f, filezip);
    return OK;

    free_:
    CLOSE_ZIP(f, filezip);
    return err;

    /*
     * in case we can not operate with the file because it's too old
    */
#ifndef ALL_VERSION
    error_version:
    CLOSE_ZIP(f, filezip);
    return ERROR_VERSION;
#endif
    error_new_version:
    CLOSE_ZIP(f, filezip);
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

int load_audio(QByteArray &array, const QString &path){
    int error;
    zip_t *file_zip;

    array.clear();

    file_zip = zip_open(path.toUtf8().constData(), 0, &error);

    if(!file_zip)
        return ERROR;

    error = xmlstruct::readFile(file_zip, array, true, NAME_AUDIO, false);

    zip_close(file_zip);
    return error;

}

size_t  xmlstruct::sizeFile(zip_t *filezip, const char *namefile){
    struct zip_stat st;
    zip_stat_init(&st);

    /*
     * Upon successful completion 0 is returned. Otherwise,
     * -1 is returned and the error information in archive
     * is set to indicate the error
    */
    if(zip_stat(filezip, namefile, 0, &st) != 0)
        return 0;

    return st.size;
}

int xmlstruct::load_file_8(Document *doc, zip_file_t *f, zip_t *filezip, const bool LoadPdf, const bool LoadImg)
{
    int tmp, ver_stroke;
    uchar controllo_parita = 0;
    fromimage::load_res res_img;

    SOURCE_READ_RETURN(f, &ver_stroke, sizeof(ver_stroke));

    SOURCE_READ_RETURN(f, &tmp, sizeof(int));
    doc->se_registato = static_cast<Document::n_audio_record>(tmp);

    LOAD_STRINGA_RETURN(f, doc->audio_position_path)

    SOURCE_READ_RETURN(f, &doc->count_pdf, sizeof(doc->count_pdf));
    SOURCE_READ_RETURN(f, &doc->count_img, sizeof(doc->count_img));

    tmp = loadbinario_3(filezip, ver_stroke);
    if(tmp == ERROR)
        return tmp;
    else if(tmp == ERROR_CONTROLL)
        /* we want to continue to load the file, but we need to return we had a problem */
        controllo_parita = 1;

    if(LoadImg){
        res_img = doc->m_img->load(filezip, f);
        if(res_img != fromimage::load_res::ok){
            return ERROR;
        }
    }

#ifdef PDFSUPPORT
    if(LoadPdf){
        auto res = doc->m_pdf->load(filezip, f, nullptr);
        if(res != frompdf::ok)
            return ERROR;
    }
#endif

    if(controllo_parita)
        return ERROR_CONTROLL;

    return OK;
}
