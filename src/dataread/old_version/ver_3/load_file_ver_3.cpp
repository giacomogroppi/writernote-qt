#include "../../xmlstruct.h"
#include "../../../datawrite/source_read_ext.h"
#include "../../../utils/common_error_definition.h"

#ifdef ALL_VERSION

int xmlstruct::load_file_3(Document *currenttitle, zip_file_t *f, zip_t *filezip)
{
    int temp;
    QString tmp_str;

    LOAD_STRINGA_RETURN(f, tmp_str);
    SOURCE_READ_RETURN(f, &temp, sizeof(int));
    currenttitle->se_registato = static_cast<Document::n_audio_record>(temp);

    SOURCE_READ_RETURN(f, &currenttitle->se_tradotto, sizeof(bool));

    LOAD_STRINGA_RETURN(f, currenttitle->testi);

    LOAD_STRINGA_RETURN(f, currenttitle->audio_position_path)

    SOURCE_READ_RETURN(f, &currenttitle->m_touch, sizeof(bool));

    if(currenttitle->m_touch){
        if(loadbinario_0(filezip) == ERROR)
            return ERROR;
    }

    LOAD_MULTIPLESTRING_RETURN(f, currenttitle->testinohtml, currenttitle->posizione_iniz);

    return OK;

}

#endif
