#include "frompdf.h"
#include "currenttitle/document.h"

#ifdef PDFSUPPORT

static inline void copy_double(const QPointF &point, double *data);

frompdf::load_res frompdf::save_metadata(zip_source_t *file)
{
    uint i;
    double pos[2];

    for(i=0; i<m_data->count_pdf; ++i){
        const Pdf &pdf = this->m_image.at(i);

        copy_double(pdf.topLeft, pos);

        if(zip_source_write(file, pos, sizeof(double)*2) == -1){
            return load_res::no_metadata;
        }
    }
    return load_res::ok;
}

/* the function append to the list a Pdf */
frompdf::load_res frompdf::load_metadata(zip_file_t *file)
{
    uint i;
    double pos[2];
    Pdf pdf;

    for(i=0; i<m_data->count_pdf; ++i){
        if(zip_fread(file, pos, sizeof(double)*2) == -1)
            return load_res::no_metadata;
        pdf.topLeft = QPointF(pos[0], pos[1]);

        this->m_image.append(pdf);
    }

    return load_res::ok;
}

void frompdf::init_FirstLoad()
{
    Pdf pdf;
    this->m_image.append(pdf);
}

static inline void copy_double(const QPointF &point, double *data){
    data[0] = point.x();
    data[1] = point.y();
}

#endif
