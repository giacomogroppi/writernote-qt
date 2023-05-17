#include "datawrite/savefile.h"
#include "utils/common_error_definition.h"
#include "images/fromimage.h"
#include "frompdf/frompdf.h"
#include "currenttitle/document.h"
#include "core/WZipCommon.h"
#include <QFile>

#define SAVE_STRINGA(x, y) if(savefile::save_string(x, y) != OK) goto delete_;

/*
 * the function save the copybook and all it's data
 * if save_audio == true -> save also the audio
*/

static size_t savefile_get_size_file(const Document *doc)
{
    size_t s = 0;
    s += sizeof(int);               // version
    s += sizeof(int);               // ver_stroke
    s += sizeof(int);               // audio
    s += sizeof(int) + doc->audio_position_path.length();
    s += sizeof(unsigned);          // len_img
    s += sizeof(unsigned);          // len_pdf

#ifdef PDFSUPPORT
    s += doc->get_size_file_pdf();
#endif // PDFSUPPORT

    s += doc->get_size_file_img();
    return s;
}

static void savefile_save_record_state(WZipWriterSingle &writer, Document *doc)
{
    int tmp = static_cast<int>(doc->se_registato);
    writer.write_object(tmp);
}

int savefile::savefile_check_file(cbool saveImg)
{
    int ver_stroke;
    fromimage::load_res_img res_img;
    WZipWriterSingle writer;

#ifdef PDFSUPPORT
    frompdf::load_res_pdf res_pdf;
#endif // PDFSUPPORT

    static_assert(sizeof(ver_stroke) == 4);

    ver_stroke = CURRENT_VERSION_STROKE;

    writer.init(nullptr, 0, savefile_get_size_file(this->_doc));

    /* remove old file formact: Version 2, 3, 4, 5 */
    WZipCommon::removeFile(_path->constData(), "indice.xml");

    writer.write_object((const int &)CURRENT_VERSION_CURRENT_TITLE);
    writer.write_object(ver_stroke);

    savefile_save_record_state(writer, _doc);

    writer.write_string(_doc->audio_position_path.toUtf8().constData(), _doc->audio_position_path.size());

#ifdef PDFSUPPORT
    writer.write_object(_doc->length_pdf());
#else
    writer.write_object(static_cast<int>(0));
#endif // PDFSUPPORT
    writer.write_object(_doc->length_img());

    res_img = _doc->save_metadata_img(writer);
    if(res_img != fromimage::load_res_img::ok)
        return ERROR;

#ifdef PDFSUPPORT
    res_pdf = _doc->save_metadata_pdf(writer);
    if(res_pdf != frompdf::load_res_pdf::ok)
        return ERROR;
#endif // PDFSUPPORT

    if(writer.commit_change(*_path, WByteArray(NAME_FILE)) < 0)
        return ERROR;

    if(this->salvabinario(saveImg) != OK){
        return ERROR;
    }

    return OK;
}

int save_audio_file(const char *posAudio,
                    const WByteArray &path)
{
    WZipWriter writer;

    if(writer.init(path.constData()) < 0)
        return ERROR;

    return savefile::moveFileIntoZip(WByteArray(posAudio), writer, NAME_AUDIO);
}
