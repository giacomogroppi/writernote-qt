#pragma once

#include <QtGlobal>

#ifndef PDFSUPPORT
class frompdf{
public:
    uchar var;
    frompdf(void *){};
};
#endif

#ifdef PDFSUPPORT

#include "touch/dataTouch/datastruct/datastruct.h"
#include "currenttitle/document.h"
#include "zip.h"
#include <QString>
#include <QList>
#include <QImage>
#include <QMap>
#include <QPainter>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
# include "poppler-qt5.h"
#else
# include "poppler-qt6.h"
#endif

#include "../images/fromimage.h"
#define IMG_PDF_HEIGHT 292
#define IMG_PDF_WIDTH 210

#define SUFFIX_PDF "_pdf_"

class TabletCanvas;

class Pdf{
public:
    Pdf()
    {
        img = QList<QImage>();
        topLeft = QPointF();
    }

    Pdf(const Pdf &pdf)
    {
        this->img = pdf.img;
        this->topLeft = pdf.topLeft;
    }

    ~Pdf() = default;

    QList<QImage> img;
    
    /* top left */
    QPointF topLeft;
};

class frompdf
{
private:
    Document *m_data;

    QVector<Pdf> m_image;

    static QByteArray getName(const unsigned i)
    {
        const auto str = SUFFIX_PDF + QString::number(i);
        return str.toUtf8();
    }

public:
    static void copy(const frompdf &src, frompdf &dest);

    static bool isvalid(QString &pos);

    void translation(const QPointF &point);
    void translation(const double x, const double y);

    frompdf(Document *doc);
    ~frompdf() = default;

    enum load_res: uchar{
        ok,
        not_valid_pdf,
        not_valid_page,
        no_metadata,
        no_valid_path
    };

    void reset();

    QList<QString> get_name_pdf();

    /* return true if all load correctly */
    bool load(const QList<QString> &path, QMap<load_res, uchar> &index, TabletCanvas *canvas);

    load_res load(const QString &, cbool clear, TabletCanvas *canvas);
    /* it load from a zip_t file all the pdf for the current copybook */

#ifdef ALL_VERSION
    load_res load(zip_t *filezip, zip_file_t *file, TabletCanvas *canvas);
#endif // ALL_VERSION
    load_res load(WZipReaderSingle &reader, TabletCanvas *canvas);
    load_res load(const QByteArray &path_writernote_file, TabletCanvas *canvas);


    load_res load_from_row(const QByteArray &, cbool clear,
                           cbool FirstLoad, const uchar IndexPdf,
                           TabletCanvas *canvas);

    void resizing(TabletCanvas *canvas, const uint lenPdf);

    [[nodiscard]] load_res save(const QList<QString> &path, const QByteArray &path_writernote_file);
    [[nodiscard]] load_res save(const QByteArray &path, const QByteArray &path_writernote_file);
    [[nodiscard]] load_res save(WZipWriter &filezip, const QByteArray &path, const QByteArray &path_writernote_file);

    load_res save_metadata(WZipWriterSingle &writer);

    void draw(QPainter &painter, const double delta, cbool IsExportingPdf) const;

    unsigned insert_pdf(QByteArray &pos, const PointSettable *point);

    void addPdf(QByteArray &pos,
                const PointSettable *point,
                const QByteArray &path_writernote,
                TabletCanvas *canvas);

    uint resolution = 500;//72
    size_t get_size_file() const;

private:
    void adjast(const uchar indexPdf);

#ifdef ALL_VERSION
    load_res load_metadata(zip_file_t *file);
#endif
    load_res load_metadata(WZipReaderSingle &reader);

    /*
     * this function only append a pdf to
     * the list
    */
    void init_FirstLoad();
};

inline void frompdf::copy(const frompdf &src, frompdf &dest)
{
    dest.m_image = src.m_image;
}

inline void frompdf::translation(const double x, const double y)
{
    translation(QPointF(x, y));
}

inline void frompdf::reset()
{
    m_image.clear();
    this->m_data->count_pdf = 0;
}

force_inline void frompdf::draw(QPainter &painter, const double delta, cbool IsExportingPdf) const
{
    int i, k, len_img;
    QRectF size;
    const Pdf *pdf;
    const int len = this->m_image.length();

    if(!len)
        return;

    double prop;
    {
        const auto &target = m_image.at(0).img.at(0);
        prop = double(target.height()) / double(target.width());
    }

    const double x = m_data->datatouch->currentWidth() * delta;
    const double y = x * prop;

    for(i = 0; i < len; ++i){
        pdf = &this->m_image.at(i);
        len_img = pdf->img.length();
        size = QRectF(pdf->topLeft*delta, QSizeF(x, y));

        for(k = 0; k < len_img; k++){
            const auto &img = pdf->img.at(k);
            fromimage::draw(painter, size, img);

            size.setY(size.y() + y);

            size.setHeight(y);

            if(unlikely(IsExportingPdf)){
                size.setWidth(x);
            }
        }
    }

}

#endif // PDFSUPPORT
