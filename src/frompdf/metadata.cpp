#include "frompdf.h"
#include "../currenttitle/document.h"

frompdf::load_res frompdf::save_metadata(zip_source_t *file)
{
    uint i;
    double pos[4];

    for(i=0; i<m_data->count_pdf; ++i){
        if(zip_source_write(file, &m_translation, sizeof(this->m_translation)) == -1){
            return load_res::no_metadata;
        }
    }
    return load_res::ok;
}

frompdf::load_res frompdf::load_metadata(zip_file_t *file)
{
    uint i;
    double pos[4];
    struct immagine_s img;

    for(i=0; i<m_data->count_pdf; ++i){
        if(zip_fread(file, pos, sizeof(double)*4) == -1)
            return load_res::no_metadata;
        img.i = QPointF(pos[0], pos[1]);
        img.f = QPointF(pos[2], pos[3]);

        m_image.append(img);
    }

    return load_res::ok;
}
