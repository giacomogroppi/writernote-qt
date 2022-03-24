#include "dataread/xmlstruct.h"
#include "datawrite/source_read_ext.h"
#include "utils/common_error_definition.h"

#ifdef ALL_VERSION

int xmlstruct::load_file_4(Document *currenttitle, zip_file_t *f, zip_t *filezip){
    int temp;
    bool tmp_touch, translate;
    uchar controllo_parita = 0;
    QString tmp_testi;
    QString tmp_str;
    LOAD_STRINGA_RETURN(f, tmp_str);

    SOURCE_READ_RETURN_SIZE(f, &temp, sizeof(int));
    currenttitle->se_registato = static_cast<Document::n_audio_record>(temp);

    SOURCE_READ_RETURN_SIZE(f, &translate, sizeof(bool));

    LOAD_STRINGA_RETURN(f, tmp_testi);

    LOAD_STRINGA_RETURN(f, currenttitle->audio_position_path)

    SOURCE_READ_RETURN_SIZE(f, &tmp_touch, sizeof(bool));

    CONTROLL_KEY(tmp_touch);

    temp = loadbinario_1(filezip);
    if(temp == ERROR){
        return temp;
    }
    else if(temp == ERROR_CONTROLL){
        /* we want to continue to load the file, but we need to return we had a problem */
        controllo_parita = 1;
    }

    if(controllo_parita)
        return ERROR_CONTROLL;

    return OK;
}

#endif // ALL_VERSION
