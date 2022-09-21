#include "frompdf.h"
#include "currenttitle/document.h"

#ifdef PDFSUPPORT

static inline void copy_double(const QPointF &point, double *data);

frompdf::load_res frompdf::save_metadata(WZipWriterSingle &writer)
{
    unsigned i;
    double pos[2];

    for(i = 0; i < m_data->count_pdf; i++){
        const Pdf &pdf = this->m_image.at(i);

        copy_double(pdf.topLeft, pos);

        static_assert(sizeof(pos) == sizeof(double) * 2);

        writer.write(pos, sizeof(pos));
    }
    return load_res::ok;
}

frompdf::load_res frompdf::load_metadata(WZipReaderSingle &reader)
{
    unsigned i;
    double pos[2];
    Pdf pdf;

    static_assert(sizeof(pos) == sizeof(double) * 2);

    for(i = 0; i < m_data->count_pdf; ++i){
        if(reader.read_by_size(pos, sizeof(pos)))
            return load_res::no_metadata;

        pdf.topLeft = QPointF(pos[0], pos[1]);

        this->m_image.append(pdf);
    }

    return load_res::ok;
}

size_t frompdf::get_size_file() const
{
    const size_t s = sizeof(double) * 2 * m_data->count_pdf;
    return s;
}

#ifdef ALL_VERSION
/* the function append to the list a Pdf */
frompdf::load_res frompdf::load_metadata(zip_file_t *file)
{
    uint i;
    double pos[2];
    Pdf pdf;

    for(i = 0; i < m_data->count_pdf; ++i){
        if(zip_fread(file, pos, sizeof(double)*2) == -1)
            return load_res::no_metadata;
        pdf.topLeft = QPointF(pos[0], pos[1]);

        this->m_image.append(pdf);
    }

    return load_res::ok;
}
#endif // ALL_VERSION

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
