#include "dataread/xmlstruct.h"
#include "datawrite/source_read_ext.h"
#include "utils/common_error_definition.h"

#ifdef ALL_VERSION

int xmlstruct::load_file_3(WZipReaderSingle &reader)
{
    int temp;
    bool tmp_touch, translate;
    QString tmp_str;
    WZip &zip = *reader.get_zip();

    if(reader.read_string(tmp_str) < 0)
        return ERROR;

    {
        if(reader.read_object(temp) < 0)
            return ERROR;

        _doc->se_registato = static_cast<Document::n_audio_record>(temp);
    }

    if(reader.read_object(translate) < 0)
        return ERROR;

    if(reader.read_string(tmp_str) < 0)
        return ERROR;

    if(reader.read_string(_doc->audio_position_path) < 0)
        return ERROR;

    if(reader.read_object(tmp_touch) < 0)
        return ERROR;

    CONTROLL_KEY(tmp_touch);

    zip.dealloc_file();

    if(loadbinario_0(zip) == ERROR)
        return ERROR;

    return OK;
}

#endif
