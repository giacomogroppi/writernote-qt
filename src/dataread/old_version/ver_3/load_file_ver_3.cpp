#include "dataread/xmlstruct.h"
#include "datawrite/source_read_ext.h"
#include "utils/common_error_definition.h"

#ifdef ALL_VERSION

int xmlstruct::load_file_3(Document *currenttitle, zip_file_t *f, zip_t *filezip)
{
    int temp;
    bool tmp_touch, translate;
    QString tmp_str;

    LOAD_STRINGA_RETURN(f, tmp_str);
    SOURCE_READ_RETURN(f, &temp, sizeof(int));
    currenttitle->se_registato = static_cast<Document::n_audio_record>(temp);

    SOURCE_READ_RETURN(f, &translate, sizeof(bool));

    LOAD_STRINGA_RETURN(f, tmp_str);

    LOAD_STRINGA_RETURN(f, currenttitle->audio_position_path)

    SOURCE_READ_RETURN(f, &tmp_touch, sizeof(bool));

    CONTROLL_KEY(tmp_touch);

    if(loadbinario_0(filezip) == ERROR)
        return ERROR;

    return OK;
}

#endif
