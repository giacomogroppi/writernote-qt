#include "dataread/xmlstruct.h"
#include "datawrite/source_read_ext.h"
#include "utils/common_error_definition.h"
#include "images/fromimage.h"
#include "frompdf/frompdf.h"

#ifdef ALL_VERSION
int xmlstruct::load_file_6(WZipReaderSingle &reader, cbool LoadPdf, cbool LoadImg)
{
    unsigned len_pdf, len_img;
    int tmp;
    uchar controllo_parita = 0;
    WZip &zip = *reader.get_zip();

    {
        if(reader.read_object(tmp) < 0)
            return ERROR;

        _doc->se_registato = static_cast<Document::n_audio_record>(tmp);
    }

    if(reader.read_string(_doc->audio_position_path) < 0)
        return ERROR;

    if(reader.read_object(len_pdf) < 0)
        return ERROR;
    if(reader.read_object(len_img) < 0)
        return ERROR;

    if(LoadImg){
        const auto res_img = _doc->m_img->load(reader, len_img);
        if(res_img != fromimage::load_res::ok){
            return ERROR;
        }
    }

#ifdef PDFSUPPORT
    if(LoadPdf){
        const auto res = _doc->m_pdf->load_pdf(reader, static_cast<int>(len_pdf), *_doc->datatouch);
        if(res != frompdf::ok)
            return ERROR;
    }
#endif

    zip.dealloc_file();

    tmp = loadbinario_1(zip);
    if(tmp == ERROR)
        return tmp;
    else if(tmp == ERROR_CONTROLL)
        /* we want to continue to load the file, but we need to return we had a problem */
        controllo_parita = 1;

    if(controllo_parita)
        return ERROR_CONTROLL;

    return OK;
}

#endif //ALL_VERSION
