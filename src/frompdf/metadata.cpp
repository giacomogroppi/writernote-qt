#include "frompdf.h"

#ifdef PDFSUPPORT

static inline void copy_double(const PointF &point, double *data);

frompdf::load_res_pdf frompdf::save_metadata_pdf(WZipWriterSingle &writer)
{
    double pos[2];
    const auto len = m_image.length();

    for(auto i = 0; i < len; i++){
        const Pdf &pdf = this->m_image.at(i);

        copy_double(pdf.topLeft, pos);

        static_assert(sizeof(pos) == sizeof(double) * 2);

        writer.write(pos, sizeof(pos));
    }
    return load_res_pdf::ok;
}

frompdf::load_res_pdf frompdf::load_metadata_pdf(WZipReaderSingle &reader, int len)
{
    int i;
    double pos[2];
    Pdf pdf;

    static_assert(sizeof(pos) == sizeof(double) * 2);

    for(i = 0; i < len; i++){
        if(reader.read_by_size(pos, sizeof(pos)))
            return load_res_pdf::no_metadata;

        pdf.topLeft = PointF(pos[0], pos[1]);

        this->m_image.append(pdf);
    }

    return load_res_pdf::ok;
}

size_t frompdf::get_size_file_pdf() const
{
    const size_t s = sizeof(double) * 2 * m_image.length();
    return s;
}

void frompdf::init_FirstLoad_pdf()
{
    Pdf pdf;
    this->m_image.append(pdf);
}

static inline void copy_double(const PointF &point, double *data)
{
    data[0] = point.x();
    data[1] = point.y();
}

#endif
