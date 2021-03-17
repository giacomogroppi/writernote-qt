#include "extract_audio.h"
#include "../../dataread/xmlstruct.h"
#include "../../currenttitle/currenttitle_class.h"
#include "../common_error_definition.h"

#define DELETE_T delete indice; \
    delete title; \
    delete m_data;

/*
 * the function extract an audio from a copybook, into path_to
*/
extract::n_extract extract_audio(const char *path, const char *namecopybook, const char *path_to){
    QString m_path = path, m_namecopybook = namecopybook, m_path_to = path_to;

    indice_class * indice = new indice_class;
    currenttitle_class * title = new currenttitle_class;

    xmlstruct * m_data = new xmlstruct(&m_path, indice, title);

    if(m_data->loadindice() != OK){
        DELETE_T;
        return extract::load_file;
    }

    if(indice->titolo.indexOf(namecopybook) == -1){
        DELETE_T;
        return extract::no_copybook;
    }

    if(m_data->loadfile(namecopybook) != OK){
        DELETE_T;
        return extract::load_file;
    }

    if(title->se_registato == audio_record::not_record){
        DELETE_T;
        return extract::not_record;
    }

    if(load_audio(&title->audio_data, m_namecopybook, m_path) != OK)
    {
        DELETE_T;
        return extract::load_audio;
    }

    FILE *fp = fopen(path_to, "w");
    if(!fp){
        DELETE_T;
        return extract::open_to;
    }

    auto size_audio = title->audio_data.size();

    fwrite(title->audio_data.data(), size_audio, 1, fp);

    fclose(fp);

    DELETE_T;

    return extract::ok;
}
