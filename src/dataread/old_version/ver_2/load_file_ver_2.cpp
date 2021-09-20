#include "../../xmlstruct.h"
#include "../../../datawrite/source_read_ext.h"
#include "../../../utils/common_error_definition.h"

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
        if(loadbinario_0(filezip) == ERROR)
            return ERROR;
    }

    LOAD_MULTIPLESTRING_RETURN(f, currenttitle->testinohtml, currenttitle->posizione_iniz)

    return OK;
}

#endif // ALL_VERSION
