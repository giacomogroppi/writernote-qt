#include "xmlstruct.h"

#include "../images/save_images.h"
#include "../datawrite/source_read_ext.h"
#include "../utils/common_error_definition.h"

#define LOAD_STRINGA(x, y) if(load_stringa(x,y) == ERROR) goto free_;
#define LOAD_STRINGA_RETURN(x, y) if(load_stringa(x, y) == ERROR)return ERROR;

static size_t sizeFile(zip_t *filezip, const char *namefile);

static int load_stringa(zip_file_t *f, QString *stringa){
    int temp;

    SOURCE_READ_RETURN(f, &temp, sizeof(int));

    if(temp){
        char *vartempp = new char[temp + 1];

        SOURCE_READ_RETURN(f, vartempp, sizeof(char)*temp);

        vartempp[temp] = '\0';

        *stringa = vartempp;

        delete [] vartempp;

    }

    return OK;
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
        LOAD_STRINGA_RETURN(f, &temp_);

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
#define LOAD_BINARIO_RETURN(x) if(loadbinario(x) == ERROR) return ERROR;

int xmlstruct::loadfile(const char *nameFile){
    currenttitle->reset();
    currenttitle->datatouch->reset();

    int err = 0;

    zip_t *filezip = zip_open(this->path_->toUtf8().constData(), 0, &err);
    if (filezip == NULL)
        return false;

    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(filezip, nameFile, 0, &st);

    zip_file_t *f = zip_fopen(filezip, nameFile, 0);
    if(f == NULL){
        zip_close(filezip);
        return false;
    }

    int temp_versione;
    SOURCE_READ_GOTO(f, &temp_versione, sizeof(int));

    if(temp_versione <= 2){
#ifdef ALL_VERSION
        if(load_file_2(currenttitle, f, filezip) != OK)
            goto free_;
#else
        goto ERROR_VERSION;
#endif
    }else if(temp_versione == 3){
        if(load_file_3(currenttitle, f, filezip) != OK)
            goto free_;

    }else if(temp_versione > 3)
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
    return ERROR_VERION_NEW;
}

#ifdef ALL_VERSION
/*
 * this version of the file did not allow to save the audio
 * inside the file, the variable if_registrato was therefore
 * of type bool, so we cannot read an internal as in the
 * following versions.
 * furthermore we are obliged to read the audio_potion_path string
*/
int xmlstruct::load_file_2(currenttitle_class *currenttitle, zip_file_t *f, zip_t *filezip){
    LOAD_STRINGA_RETURN(f, &currenttitle->nome_copybook);

    bool temp;
    SOURCE_READ_RETURN(f, &temp, sizeof(bool));
    if(temp)
        this->currenttitle->se_registato = audio_record::record_file;
    else
        this->currenttitle->se_registato = audio_record::not_record;

    SOURCE_READ_RETURN(f, &currenttitle->se_tradotto, sizeof(bool));

    LOAD_STRINGA_RETURN(f, &currenttitle->testi)

    LOAD_STRINGA_RETURN(f, &currenttitle->audio_position_path)

    SOURCE_READ_RETURN(f, &currenttitle->m_touch, sizeof(bool));

    if(currenttitle->m_touch){
        LOAD_BINARIO_RETURN(filezip);
    }

    LOAD_MULTIPLESTRING_RETURN(f, &currenttitle->testinohtml, &currenttitle->posizione_iniz)

    LOAD_IMAGE_RETURN(&currenttitle->immagini, f);

    return OK;
}
#endif

#define LOAD_AUDIO(x, y, z) if(load_audio(x, y, z) != OK) return ERROR;

int xmlstruct::load_file_3(currenttitle_class *currenttitle, zip_file_t *f, zip_t *filezip)
{
    LOAD_STRINGA_RETURN(f, &currenttitle->nome_copybook);

    int temp;
    SOURCE_READ_RETURN(f, &temp, sizeof(int));
    currenttitle->se_registato = static_cast<audio_record::n_audio_record>(temp);

    SOURCE_READ_RETURN(f, &currenttitle->se_tradotto, sizeof(bool));

    LOAD_STRINGA_RETURN(f, &currenttitle->testi);

    LOAD_STRINGA_RETURN(f, &currenttitle->audio_position_path)

    SOURCE_READ_RETURN(f, &currenttitle->m_touch, sizeof(bool));

    if(currenttitle->m_touch){
        LOAD_BINARIO_RETURN(filezip);
    }

    LOAD_MULTIPLESTRING_RETURN(f, &currenttitle->testinohtml, &currenttitle->posizione_iniz);

    LOAD_IMAGE_RETURN(&currenttitle->immagini, f);

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
    zip_close(y); \
    if(audio_data) free(audio_data);

#define NAME_AUDIO(x) (x+"audio.wav")

int load_audio(QByteArray *array, QString &namecopybook, QString &path){
    int error;
    void * audio_data = nullptr;

    array->clear();

    zip_t *file_zip = zip_open(path.toUtf8().constData(), 0, &error);

    if(!file_zip)
        return ERROR;

    size_t size_audio = sizeFile(file_zip, NAME_AUDIO(namecopybook).toUtf8().constData());

    if(!size_audio)
        return ERROR;

    zip_file_t *f;
    f = zip_fopen(file_zip, NAME_AUDIO(namecopybook).toUtf8().constData(), 0);
    if(f == NULL)
        return ERROR;

    audio_data = malloc(size_audio);
    if(!audio_data)
        goto free_;

    SOURCE_READ_GOTO(f, audio_data, size_audio);

    array->append((const char *)audio_data, size_audio);


    CLOSE_ZIP_AUDIO(f, file_zip);
    return OK;

    free_:
    CLOSE_ZIP_AUDIO(f, file_zip);
    return ERROR;

}

static size_t sizeFile(zip_t *filezip, const char *namefile){
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
