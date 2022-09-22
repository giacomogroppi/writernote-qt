#include "datawrite/savefile.h"
#include "datawrite/source_read_ext.h"
#include "utils/common_error_definition.h"
#include "images/fromimage.h"
#include "frompdf/frompdf.h"
#include "currenttitle/document.h"
#include "core/WZipCommon.h"
#include <QFile>

#define SAVE_STRINGA(x, y) if(savefile::save_string(x, y) != OK) goto delete_;

static void setCurrentVersion(Document *data)
{
    data->versione = CURRENT_VERSION_CURRENT_TITLE;
}

/*
 * the function save the copybook and all it's data
 * if save_audio == true -> save also the audio
*/

static size_t savefile_get_size_file(const Document *doc)
{
    size_t s = 0;
    s += sizeof(doc->versione);
    s += sizeof(int); // ver_stroke
    s += sizeof(int); // audio
    s += sizeof(int) + doc->audio_position_path.length();
    s += sizeof(doc->count_img);
    s += sizeof(doc->count_pdf);
    s += doc->m_pdf->get_size_file();
    s += doc->m_img->get_size_file();
    return 0;
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
    fromimage::load_res res_img;
    WZipWriterSingle writer;

#ifdef PDFSUPPORT
    frompdf::load_res res_pdf;
#endif // PDFSUPPORT

    static_assert(sizeof(_doc->versione) == 4);
    static_assert(sizeof(ver_stroke) == 4);
    static_assert(sizeof(_doc->count_img) == sizeof(_doc->count_pdf));
    static_assert(sizeof(_doc->count_img) == 4);

    setCurrentVersion(_doc);
    ver_stroke = CURRENT_VERSION_STROKE;

    writer.init(NULL, 0, savefile_get_size_file(this->_doc));

    /* remove old file formact: Version 2, 3, 4, 5 */
    WZipCommon::removeFile(_path->toUtf8().constData(), "indice.xml");

    writer.write_object(_doc->versione);
    writer.write_object(ver_stroke);

    savefile_save_record_state(writer, _doc);

    writer.write_string(_doc->audio_position_path.toUtf8().constData(), _doc->audio_position_path.size());

    writer.write_object(_doc->count_pdf);
    writer.write_object(_doc->count_img);

    res_img = _doc->m_img->save_metadata(writer);
    if(res_img != fromimage::load_res::ok)
        return ERROR;

#ifdef PDFSUPPORT
    res_pdf = _doc->m_pdf->save_metadata(writer);
    if(res_pdf != frompdf::load_res::ok)
        return ERROR;
#endif // PDFSUPPORT

    if(writer.commit_change(*_path, QByteArray(NAME_FILE)) < 0)
        return ERROR;

    if(this->salvabinario(saveImg) != OK){
        return ERROR;
    }

    return OK;
}

uchar savefile::save_string(zip_source_t *file, const char *stringa)
{
    int size = strlen(stringa);
    SOURCE_WRITE_RETURN_SIZE(file, &size, sizeof(size));

    SOURCE_WRITE_RETURN_SIZE(file, stringa, sizeof(char)*size);

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
