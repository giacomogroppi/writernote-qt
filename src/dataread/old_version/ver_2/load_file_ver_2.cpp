#include "dataread/xmlstruct.h"
#include "datawrite/source_read_ext.h"
#include "utils/common_error_definition.h"
#include "core/WZipReaderSingle.h"

#ifdef ALL_VERSION
/*
 * this version of the file did not allow to save the audio
 * inside the file, the variable if_registrato was therefore
 * of type bool, so we cannot read an internal as in the
 * following versions.
 * furthermore we are obliged to read the audio_potion_path string
*/
int xmlstruct::load_file_2(WZipReaderSingle &reader)
{
    QString tmp_str, tmp_testi;
    bool temp, translate;
    bool tmp_touch;
    WZip &zip = *reader.get_zip();

    if(reader.read_string(tmp_str) < 0)
        return ERROR;

    {
        if(reader.read_object(temp) < 0)
            return ERROR;

        if(temp)
            this->_doc->se_registato = Document::record_file;
        else
            this->_doc->se_registato = Document::not_record;
    }

    if(reader.read_object(translate) < 0)
        return ERROR;

    if(reader.read_string(tmp_testi) < 0)
        return ERROR;

    if(reader.read_string(_doc->audio_position_path) < 0)
        return ERROR;

    if(reader.read_object(tmp_touch) < 0)
        return ERROR;

    CONTROLL_KEY(tmp_touch);

    zip.dealloc_file();

    if(this->loadbinario_0(zip) == ERROR)
        return ERROR;

    return OK;
}

#endif // ALL_VERSION
