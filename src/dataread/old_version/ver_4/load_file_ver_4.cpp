#include "../../xmlstruct.h"
#include "../../../datawrite/source_read_ext.h"
#include "../../../utils/common_error_definition.h"

#ifdef ALL_VERSION

int xmlstruct::load_file_4(Document *currenttitle, zip_file_t *f, zip_t *filezip){
    int temp;
    uchar controllo_parita = 0;
    QString tmp_str;
    LOAD_STRINGA_RETURN(f, tmp_str);

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

    LOAD_MULTIPLESTRING_RETURN(f, currenttitle->testinohtml, currenttitle->posizione_iniz);

    if(controllo_parita)
        return ERROR_CONTROLL;

    return OK;
}

#endif // ALL_VERSION
