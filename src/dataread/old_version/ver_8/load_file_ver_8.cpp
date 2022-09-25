#include "dataread/xmlstruct.h"
#include "images/fromimage.h"
#include "datawrite/source_read_ext.h"
#include "utils/common_error_definition.h"
#include "frompdf/frompdf.h"

#ifdef ALL_VERSION

int xmlstruct::load_file_8(WZipReaderSingle &reader, cbool LoadPdf, cbool LoadImg)
{
    int tmp, ver_stroke;
    uchar controllo_parita = 0;
    fromimage::load_res res_img;
    WZip &zip = *reader.get_zip();

    if(reader.read_object(ver_stroke) < 0)
        return ERROR;

    {
        if(reader.read_object(tmp) < 0)
            return ERROR;
        _doc->se_registato = static_cast<Document::n_audio_record>(tmp);
    }

    if(reader.read_string(_doc->audio_position_path) < 0)
        return ERROR;

    if(reader.read_object(_doc->count_pdf) < 0)
        return ERROR;

    if(reader.read_object(_doc->count_img) < 0)
        return ERROR;


    tmp = loadbinario_3(filezip, ver_stroke);
    if(tmp == ERROR)
        return tmp;
    else if(tmp == ERROR_CONTROLL)
        /* we want to continue to load the file, but we need to return we had a problem */
        controllo_parita = 1;

    if(LoadImg){
        res_img = doc->m_img->load(filezip, f);
        if(res_img != fromimage::load_res::ok){
            return ERROR;
        }
    }

#ifdef PDFSUPPORT
    if(LoadPdf){
        auto res = doc->m_pdf->load(filezip, f, nullptr);
        if(res != frompdf::ok)
            return ERROR;
    }
#endif

    if(controllo_parita)
        return ERROR_CONTROLL;

    return OK;
}

#endif //ALL_VERSION
