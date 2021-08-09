#include "xmlstruct.h"
#include "../images/fromimage.h"
#include "../datawrite/source_read_ext.h"
#include "../utils/common_error_definition.h"
#include "../frompdf/frompdf.h"

#define LOAD_STRINGA(x, y) if(xmlstruct::load_stringa(x,y) == ERROR) goto free_;
#define LOAD_STRINGA_RETURN(x, y) if(xmlstruct::load_stringa(x, y) == ERROR)return ERROR;

int xmlstruct::load_stringa(zip_file_t *f, QString &stringa){
    int temp;

    SOURCE_READ_RETURN(f, &temp, sizeof(int));

    if(temp){
        char *vartempp = new char[temp + 1];

        SOURCE_READ_RETURN(f, vartempp, sizeof(char)*temp);

        vartempp[temp] = '\0';

        stringa = vartempp;

        delete [] vartempp;

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

#define LOAD_MULTIPLESTRING(x, y, z) if(load_multiplestring(x,y,z) == ERROR) goto free_;
#define LOAD_MULTIPLESTRING_RETURN(x, y, z) if(load_multiplestring(x,y,z) == ERROR) return ERROR;

static int load_multiplestring(zip_file_t *f, QList<QString> * lista, QList<int> * data){
    int i, lunghezza, temp;

    SOURCE_READ_RETURN(f, &lunghezza, sizeof(int));
    if(!lunghezza)
        return OK;


    QString temp_;

    for(i=0; i<lunghezza; i++){
        LOAD_STRINGA_RETURN(f, temp_);

        lista->append(temp_);
    }

    for(i=0; i<lunghezza; i++){
        SOURCE_READ_RETURN(f, &temp, sizeof(int));

        data->append(temp);
    }

    return OK;
}

#define CLOSE_ZIP(x, y) zip_fclose(x);zip_close(y);
#define LOAD_IMAGE(x,y) if(load_image(x, y) != OK)goto free_;
#define LOAD_IMAGE_RETURN(x, y) if(load_image(x, y) != OK) return ERROR;

#define LOAD_BINARIO(x) if(loadbinario(x) == ERROR) goto free_;
#define LOAD_BINARIO_RETURN(x, function) if(function(x) == ERROR) return ERROR;

int xmlstruct::loadfile(const char *nameFile){
    currenttitle->reset();
    int err = 0;
    zip_t *filezip;
    zip_file_t *f;
    struct zip_stat st;
    int tmp_ver;

    filezip = zip_open(path_->toUtf8().constData(),
                       ZIP_CREATE,
                       &err);

    if (filezip == NULL)
        return false;

    zip_stat_init(&st);
    zip_stat(filezip, nameFile, 0, &st);

    f = zip_fopen(filezip, nameFile, 0);
    if(f == NULL){
        zip_close(filezip);
        return false;
    }

    SOURCE_READ_GOTO(f, &tmp_ver, sizeof(int));

    if(tmp_ver <= 2){
#ifdef ALL_VERSION
        if(load_file_2(currenttitle, f, filezip) != OK)
            goto free_;
    }else if(tmp_ver == 3){
        if(load_file_3(currenttitle, f, filezip) != OK)
            goto free_;

    }else if(tmp_ver == 4){
        if(load_file_4(currenttitle, f, filezip) != OK)
            goto free_;
    }
#else
        goto ERROR_VERSION;
#endif
    else if(tmp_ver == 5){
        if(load_file_5(currenttitle, f, filezip) != OK)
            goto free_;
    }
    else if(tmp_ver > 5)
        goto error_new_version;


    /*
     * after we load all the file we
     * set the version of the new
     * in this way, even the files created
     * by previous versions of writernote
     * will be updated to the new versions
    */
    currenttitle->versione = CURRENT_VERSION_CURRENT_TITLE;

    CLOSE_ZIP(f, filezip);
    return OK;

    free_:
    CLOSE_ZIP(f, filezip);
    return ERROR;

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

#define LOAD_AUDIO(x, y, z) if(load_audio(x, y, z) != OK) return ERROR;


#ifdef ALL_VERSION
/*
 * this version of the file did not allow to save the audio
 * inside the file, the variable if_registrato was therefore
 * of type bool, so we cannot read an internal as in the
 * following versions.
 * furthermore we are obliged to read the audio_potion_path string
*/
int xmlstruct::load_file_2(Document *currenttitle, zip_file_t *f, zip_t *filezip){
    LOAD_STRINGA_RETURN(f, currenttitle->nome_copybook);

    bool temp;
    SOURCE_READ_RETURN(f, &temp, sizeof(bool));
    if(temp)
        this->currenttitle->se_registato = Document::record_file;
    else
        this->currenttitle->se_registato = Document::not_record;

    SOURCE_READ_RETURN(f, &currenttitle->se_tradotto, sizeof(bool));

    LOAD_STRINGA_RETURN(f, currenttitle->testi)

    LOAD_STRINGA_RETURN(f, currenttitle->audio_position_path)

    SOURCE_READ_RETURN(f, &currenttitle->m_touch, sizeof(bool));

    if(currenttitle->m_touch){
        LOAD_BINARIO_RETURN(filezip, loadbinario_0);
    }

    LOAD_MULTIPLESTRING_RETURN(f, &currenttitle->testinohtml, &currenttitle->posizione_iniz)

    LOAD_IMAGE_RETURN(&currenttitle->immagini, f);

    return OK;
}

int xmlstruct::load_file_3(Document *currenttitle, zip_file_t *f, zip_t *filezip)
{
    LOAD_STRINGA_RETURN(f, currenttitle->nome_copybook);

    int temp;
    SOURCE_READ_RETURN(f, &temp, sizeof(int));
    currenttitle->se_registato = static_cast<Document::n_audio_record>(temp);

    SOURCE_READ_RETURN(f, &currenttitle->se_tradotto, sizeof(bool));

    LOAD_STRINGA_RETURN(f, currenttitle->testi);

    LOAD_STRINGA_RETURN(f, currenttitle->audio_position_path)

    SOURCE_READ_RETURN(f, &currenttitle->m_touch, sizeof(bool));

    if(currenttitle->m_touch){
        LOAD_BINARIO_RETURN(filezip, loadbinario_0);
    }

    LOAD_MULTIPLESTRING_RETURN(f, &currenttitle->testinohtml, &currenttitle->posizione_iniz);

    LOAD_IMAGE_RETURN(&currenttitle->immagini, f);

    return OK;

}

int xmlstruct::load_file_4(Document *currenttitle, zip_file_t *f, zip_t *filezip){
    LOAD_STRINGA_RETURN(f, currenttitle->nome_copybook);

    int temp;
    uchar controllo_parita = 0;

    SOURCE_READ_RETURN(f, &temp, sizeof(int));
    currenttitle->se_registato = static_cast<Document::n_audio_record>(temp);

    SOURCE_READ_RETURN(f, &currenttitle->se_tradotto, sizeof(bool));

    LOAD_STRINGA_RETURN(f, currenttitle->testi);

    LOAD_STRINGA_RETURN(f, currenttitle->audio_position_path)

    SOURCE_READ_RETURN(f, &currenttitle->m_touch, sizeof(bool));

    if(currenttitle->m_touch){
        temp = loadbinario_1(filezip);
        if(temp == ERROR){
            return temp;
        }
        else if(temp == ERROR_CONTROLL){
            /* we want to continue to load the file, but we need to return we had a problem */
            controllo_parita = 1;
        }
    }

    LOAD_MULTIPLESTRING_RETURN(f, &currenttitle->testinohtml, &currenttitle->posizione_iniz);

    LOAD_IMAGE_RETURN(&currenttitle->immagini, f);

    if(controllo_parita)
        return ERROR_CONTROLL;

    return OK;
}

#endif

int xmlstruct::load_file_5(Document *doc, zip_file_t *f, zip_t *filezip)
{
    LOAD_STRINGA_RETURN(f, doc->nome_copybook);

    int tmp;
    uchar controllo_parita = 0;
    frompdf::load_res res;

    SOURCE_READ_RETURN(f, &tmp, sizeof(int));
    doc->se_registato = static_cast<Document::n_audio_record>(tmp);

    SOURCE_READ_RETURN(f, &doc->se_tradotto, sizeof(doc->se_tradotto));

    LOAD_STRINGA_RETURN(f, doc->testi);

    LOAD_STRINGA_RETURN(f, doc->audio_position_path)

    SOURCE_READ_RETURN(f, &doc->m_touch, sizeof(doc->m_touch));
    SOURCE_READ_RETURN(f, &doc->count_pdf, sizeof(doc->count_pdf));

    res = doc->m_pdf->load(filezip, true);
    if(res != frompdf::ok)
        return ERROR;


    if(doc->m_touch){
        tmp = loadbinario_1(filezip);
        if(tmp == ERROR){
            return tmp;
        }
        else if(tmp == ERROR_CONTROLL){
            /* we want to continue to load the file, but we need to return we had a problem */
            controllo_parita = 1;
        }
    }

    LOAD_MULTIPLESTRING_RETURN(f, &currenttitle->testinohtml, &currenttitle->posizione_iniz);

    LOAD_IMAGE_RETURN(&currenttitle->immagini, f);

    if(controllo_parita)
        return ERROR_CONTROLL;

    return OK;
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

#define NAME_AUDIO(x) (x+"audio.wav")

int load_audio(QByteArray &array, const QString &namecopybook, const QString &path){
    int error;
    zip_t *file_zip;

    array.clear();

    file_zip = zip_open(path.toUtf8().constData(), 0, &error);

    if(!file_zip)
        return ERROR;

    error = xmlstruct::readFile(file_zip, array, true, NAME_AUDIO(namecopybook), false);

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
