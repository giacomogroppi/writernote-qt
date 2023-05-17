#include "dataread/xmlstruct.h"
#include "datawrite/source_read_ext.h"
#include "utils/common_error_definition.h"

int xmlstruct::load_file_4(WZipReaderSingle &reader)
{
    bool tmp_touch, translate;
    unsigned char controllo_parita = 0;
    WString tmp_testi;
    WString tmp_str;
    WZip &zip = *reader.get_zip();

    if(reader.read_string(tmp_str) < 0)
        return ERROR;

    {
        int tmp_audio;
        static_assert(sizeof(tmp_audio) == sizeof(int));
        if(reader.read_object(tmp_audio) < 0)
            return ERROR;

        _doc->se_registato = static_cast<Document::n_audio_record>(tmp_audio);
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

    const auto res = loadbinario_1(zip);
    if(res == ERROR){
        return res;
    }
    else if(res == ERROR_CONTROLL){
        /* we want to continue to load the file, but we need to return we had a problem */
        controllo_parita = 1;
    }

    if(controllo_parita)
        return ERROR_CONTROLL;

    return OK;
}
