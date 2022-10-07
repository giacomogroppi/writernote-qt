#include "datawrite/savefile.h"
#include "datawrite/source_read_ext.h"
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
    s += doc->get_size_file_pdf();
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
    int error, ver_stroke;
    zip_error_t errore;
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

    writer.write_object(_doc->length_pdf());
    writer.write_object(_doc->length_img());

    res_img = _doc->save_metadata_img(writer);
    if(res_img != fromimage::load_res_img::ok)
        return ERROR;

#ifdef PDFSUPPORT
    res_pdf = _doc->save_metadata_pdf(writer);
    if(res_pdf != frompdf::load_res_pdf::ok)
        return ERROR;
#endif // PDFSUPPORT

    if(writer.commit_change(*_path, QByteArray(NAME_FILE)) < 0)
        return ERROR;

    if(this->salvabinario(saveImg) != OK){
        return ERROR;
    }

    return OK;
}

int save_audio_file(const char *posAudio,
                    const QByteArray &path)
{
    WZipWriter writer;

    if(writer.init(path.constData()) < 0)
        return ERROR;

    return savefile::moveFileIntoZip(QByteArray(posAudio), writer, NAME_AUDIO);
}
