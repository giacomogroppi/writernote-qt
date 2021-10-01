#include "extract_audio.h"
#include "../../dataread/xmlstruct.h"
#include "../../currenttitle/document.h"
#include "../common_error_definition.h"

/*
 * the function extract an audio from a copybook, into path_to
*/
extract::n_extract extract_audio(const QString &path,
                                 const QString &path_to){
    Document doc;
    size_t size_audio;
    xmlstruct m_data(path, doc);
    FILE *fp;

    if(m_data.loadfile(false, false) != OK){
        return extract::load_file;
    }

    if(doc.se_registato == Document::not_record){
        return extract::not_record;
    }

    if(load_audio(doc.audio_data, path) != OK){
        return extract::load_audio;
    }

    fp = fopen(path_to.toUtf8().constData(), "w");
    if(!fp){
        return extract::open_to;
    }

    size_audio = doc.audio_data.size();

    fwrite(doc.audio_data.data(), size_audio, 1, fp);

    fclose(fp);

    return extract::ok;
}
