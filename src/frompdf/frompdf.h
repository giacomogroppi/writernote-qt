#pragma once

#include "touch/dataTouch/datastruct/DataStruct.h"

#ifndef PDFSUPPORT
class frompdf{
public:
    uchar var;
    frompdf(void *){};
};
#else

#include "images/fromimage.h"
#define IMG_PDF_HEIGHT 292
#define IMG_PDF_WIDTH 210

#define SUFFIX_PDF "_pdf_"

class TabletCanvas;

class Pdf{
public:
    Pdf()
    {
        img = WListFast<WImage>();
        topLeft = PointF();
    }

    Pdf(const Pdf &pdf)
    {
        this->img = pdf.img;
        this->topLeft = pdf.topLeft;
    }

    ~Pdf() = default;

    WListFast<WImage> img;
    
    /* top left */
    PointF topLeft;
};

class frompdf
{
private:
    WVector<Pdf> m_image;

    static WByteArray getName_pdf(const unsigned i)
    {
        const auto str = WString(SUFFIX_PDF) + WString::number(i);
        return str.toUtf8();
    }

public:
    static void copy_pdf(const frompdf &src, frompdf &dest);

    void translation_pdf(const PointF &point);
    void translation_pdf(double x, double y);

    frompdf();
    ~frompdf() = default;

    enum load_res_pdf: uchar{
        ok,
        not_valid_pdf,
        not_valid_page,
        no_metadata,
        no_valid_path
    };

    void reset_pdf();

    WListFast<QString> get_name_pdf(int countPdf);

    /* return true if all load correctly */
    bool load_pdf(const WListFast<QString> &path, QMap<load_res_pdf, uchar> &index, datastruct &data);
    load_res_pdf load_pdf(const QString &, cbool clear, datastruct &data);
    load_res_pdf load_pdf(WZipReaderSingle &reader, int len, datastruct &data);
    load_res_pdf load_pdf(const QByteArray &path_writernote_file, int len, datastruct &data);
    load_res_pdf load_from_row_pdf(const QByteArray &, cbool clear, cbool FirstLoad, uchar IndexPdf, datastruct &data);

    void resizing_pdf(datastruct &d, int len);

    [[nodiscard]] load_res_pdf save_pdf(const WListFast<QString> &path, const QByteArray &path_writernote_file);
    [[nodiscard]] load_res_pdf save_pdf(const QByteArray &path, const QByteArray &path_writernote_file);
    [[nodiscard]] load_res_pdf save_pdf(WZipWriter &filezip, const QByteArray &path);

    load_res_pdf save_metadata_pdf(WZipWriterSingle &writer);

    void draw_pdf(WPainter &painter, double delta, cbool IsExportingPdf, double currentWidth) const;

    unsigned insert_pdf(QByteArray &pos, const PointSettable *point);

    void addPdf(QByteArray &pos, const PointSettable *point, const QByteArray &path_writernote, datastruct &data);

    [[nodiscard]] int length_pdf() const;

    uint resolution = 500;//72
    [[nodiscard]] size_t get_size_file_pdf() const;

private:
    void adjast_pdf(uchar indexPdf);

    load_res_pdf load_metadata_pdf(WZipReaderSingle &reader, int len);

    /*
     * this function only append a pdf to
     * the list
    */
    void init_FirstLoad_pdf();
};

inline int frompdf::length_pdf() const
{
    return this->m_image.length();
}

inline void frompdf::copy_pdf(const frompdf &src, frompdf &dest)
{
    dest.m_image = src.m_image;
}

inline void frompdf::translation_pdf(const double x, const double y)
{
    translation_pdf(PointF(x, y));
}

force_inline void frompdf::draw_pdf(WPainter &painter, const double delta, cbool IsExportingPdf, double currentWidth) const
{
    int k, len_img;
    RectF size;
    const Pdf *pdf;
    const auto len = this->m_image.length();

    if(!len)
        return;

    double prop;
    {
        const auto &target = m_image.at(0).img.at(0);
        prop = double(target.height()) / double(target.width());
    }

    const double x = currentWidth * delta;
    const double y = x * prop;

    for(auto i = 0; i < len; ++i){
        pdf = &this->m_image.at(i);
        len_img = pdf->img.length();
        size = RectF(pdf->topLeft*delta, QSizeF(x, y));

        for(k = 0; k < len_img; k++){
            const auto &img = pdf->img.at(k);
            fromimage::draw_img(painter, size, img);

            size.setY(size.y() + y);

            size.setHeight(y);

            if(un(IsExportingPdf)){
                size.setWidth(x);
            }
        }
    }

}

#endif // PDFSUPPORT
