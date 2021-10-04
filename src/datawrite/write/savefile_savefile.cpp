#include "../savefile.h"
#include "../source_read_ext.h"
#include "../../utils/common_error_definition.h"
#include "../../images/fromimage.h"
#include "../../frompdf/frompdf.h"
#include "../../currenttitle/document.h"
#include "../../indice_class.h"
#include <QFile>

#define SAVE_STRINGA(x, y) if(savefile::save_string(x, y) != OK) goto delete_;

static void setCurrentVersion(Document *data);
/*
 * the function save the copybook and all it's data
 * if save_audio == true -> save also the audio
*/

int savefile::savefile_check_file(){
    setCurrentVersion(currenttitle);
    int error, check;
    zip_error_t errore;

    fromimage::load_res res_img;

#ifdef PDFSUPPORT
    frompdf::load_res res_pdf;
#endif // PDFSUPPORT

    zip_t *filezip;
    zip_source_t *file;

    filezip = zip_open(path->toUtf8().constData(), ZIP_CREATE, &error);

    if(!filezip)
        return ERROR;

    /* remove old file formact */
    savefile::removeFile(filezip, "indice.xml");

    file = zip_source_buffer_create(0, 0, 0, &errore);
    if(!file){
        zip_close(filezip);
        return ERROR;
    }

    zip_source_begin_write(file);

    SOURCE_WRITE(file, &currenttitle->versione, sizeof(currenttitle->versione))

    {
        int temp = static_cast<int>(currenttitle->se_registato);
        SOURCE_WRITE(file, &temp, sizeof(int))
    }

    SAVE_STRINGA(file, currenttitle->audio_position_path.toUtf8().constData())

    SOURCE_WRITE(file, &currenttitle->count_pdf, sizeof(currenttitle->count_pdf));
    SOURCE_WRITE(file, &currenttitle->count_img, sizeof(currenttitle->count_img));

    SAVE_BINARY(filezip);

    res_img = currenttitle->m_img->save_metadata(file);
    if(res_img != fromimage::load_res::ok)
        goto delete_;

#ifdef PDFSUPPORT
    res_pdf = currenttitle->m_pdf->save_metadata(file);
    if(res_pdf != frompdf::load_res::ok)
        goto delete_;
#endif // PDFSUPPORT
    check = 0;

    /*
     * Upon successful completion 0 is returned. Otherwise, -1 is returned
     * and the error information in source is set to indicate the error.
    */
    check += zip_source_commit_write(file)==ERROR_PRIVATE;

    /*
     * Upon successful completion, zip_file_add() returns the index of
     * the new file in the archive, and zip_file_replace() returns 0.
     * Otherwise, -1 is returned and the error code in archive is set
     * to indicate the error.
    */
    check += zip_file_add(filezip,
                 NAME_FILE,
                 file,
                 ZIP_FL_OVERWRITE)==ERROR_PRIVATE;

    if(check != OK_PRIVATE)
        goto delete_;


    zip_close(filezip);
    return OK;

    delete_:
        zip_source_free(file);
        zip_close(filezip);
        return ERROR;
}

uchar savefile::saveArrIntoFile(const QByteArray &arr, const QString &path)
{
    FILE *fp;
    uchar byte;
    uint i, len;

    len = arr.length();
    fp = fopen(path.toUtf8().constData(), "w");

    if(!fp)
        return ERROR;

    for(i=0; i<len; ++i){
        byte = arr.at(i);

        if(fwrite(&byte, sizeof(byte), 1, fp)<1)
            return ERROR;
    }

    fclose(fp);
    return OK;
}

uchar savefile::save_string(zip_source_t *file, const char *stringa){
    int size = strlen(stringa);
    SOURCE_WRITE_RETURN(file, &size, sizeof(size));

    SOURCE_WRITE_RETURN(file, stringa, sizeof(char)*size);

    return OK;
}

int save_audio_file(const char *posAudio,
                    const QString &path){
    return savefile::saveArrayIntoFile((const QString)posAudio, path, nullptr, NAME_AUDIO, true);

}

static void setCurrentVersion(Document *data){
    data->versione = CURRENT_VERSION_CURRENT_TITLE;
}
