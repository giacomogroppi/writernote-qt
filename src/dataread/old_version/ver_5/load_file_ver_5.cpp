#include "../../xmlstruct.h"
#include "../../../datawrite/source_read_ext.h"
#include "../../../utils/common_error_definition.h"
#include "../../../images/fromimage.h"
#include "../../../frompdf/frompdf.h"

#ifdef ALL_VERSION
int xmlstruct::load_file_5(Document *doc, zip_file_t *f, zip_t *filezip,
                           const bool LoadPdf, const bool LoadImg)
{
    int tmp;
    uchar controllo_parita = 0;
    fromimage::load_res res_img;
    QString tmp_str, tmp_testi;
    bool tmp_touch, translate;

    LOAD_STRINGA_RETURN(f, tmp_str);


    SOURCE_READ_RETURN(f, &tmp, sizeof(int));
    doc->se_registato = static_cast<Document::n_audio_record>(tmp);

    SOURCE_READ_RETURN(f, &translate, sizeof(translate));

    LOAD_STRINGA_RETURN(f, tmp_testi);

    LOAD_STRINGA_RETURN(f, doc->audio_position_path)

    SOURCE_READ_RETURN(f, &tmp_touch, sizeof(tmp_touch));
    CONTROLL_KEY(tmp_touch);
    SOURCE_READ_RETURN(f, &doc->count_pdf, sizeof(doc->count_pdf));
    SOURCE_READ_RETURN(f, &doc->count_img, sizeof(doc->count_img));

    tmp = loadbinario_1(filezip);
    if(tmp == ERROR){
        return tmp;
    }
    else if(tmp == ERROR_CONTROLL){
        /* we want to continue to load the file, but we need to return we had a problem */
        controllo_parita = 1;
    }

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

#endif // ALL_VERSION
