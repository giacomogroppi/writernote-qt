#include "frompdf.h"
#include "../currenttitle/document.h"

static inline void copy(const QPointF &point, double *data);

frompdf::load_res frompdf::save_metadata(zip_source_t *file)
{
    uint i;
    double pos[4];

    for(i=0; i<m_data->count_pdf; ++i){
        const Pdf &pdf = this->m_image.at(i);

        copy(pdf.topLeft, pos);
        copy(pdf.bottomRigth, &pos[2]);

        if(zip_source_write(file, pos, sizeof(double)*4) == -1){
            return load_res::no_metadata;
        }
    }
    return load_res::ok;
}

frompdf::load_res frompdf::load_metadata(zip_file_t *file)
{
    uint i;
    double pos[4];
    Pdf pdf;

    //const QPointF size = this->m_data->datatouch->get_size_page();

    for(i=0; i<m_data->count_pdf; ++i){
        if(zip_fread(file, pos, sizeof(double)*4) == -1)
            return load_res::no_metadata;
        pdf.topLeft = QPointF(pos[0], pos[1]);
        pdf.bottomRigth = QPointF(pos[2], pos[3]);

        this->m_image.append(pdf);
    }

    return load_res::ok;
}

static inline void copy(const QPointF &point, double *data){
    data[0] = point.x();
    data[1] = point.y();
}
