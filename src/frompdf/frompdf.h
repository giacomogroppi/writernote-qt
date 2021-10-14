#ifndef FROMPDF_H
#define FROMPDF_H

#include <QtGlobal>

#ifndef PDFSUPPORT
class frompdf{
public:
    uchar var;
    frompdf(void *null){Q_UNUSED(null)};
};
#endif

#ifdef PDFSUPPORT

#include "../touch/datastruct/datastruct.h"
#include "../currenttitle/document.h"
#include "zip.h"
#include <QString>
#include <QList>
#include <QImage>
#include <QMap>
#include <QPainter>

#include "poppler-qt5.h"
#include "../images/fromimage.h"
#define IMG_PDF_HEIGHT 292
#define IMG_PDF_WIDTH 210

#define SUFFIX_PDF "_pdf_"

class TabletCanvas;

class Pdf{
public:
    QList<QImage> img;
    
    /* top left */
    QPointF topLeft;
};

class frompdf
{
private:
    Document *m_data;

    Poppler::Document *doc = nullptr;
    QList<Pdf> m_image;

    static inline QString getName(const uint i){
        return SUFFIX_PDF + QString::number(uint(i));
    }

public:
    static void copy(const frompdf &src, frompdf &dest){
        dest.m_image = src.m_image;
        dest.doc = nullptr;
    }

    static bool isvalid(QString &pos);

    void translation(const QPointF &point);
    inline void translation(const double x, const double y){
        translation(QPointF(x, y));
    }

    frompdf(Document *doc);
    ~frompdf(){
        if(this->doc)
            delete doc;
    }

    enum load_res: uchar{
        ok,
        not_valid_pdf,
        not_valid_page,
        no_metadata,
        no_valid_path
    };

    inline void reset(){
        m_image.clear();
        this->m_data->count_pdf = 0;
    }

    QStringList get_name_pdf();

    /* return true if all load correctly */
    bool load(const QStringList &path, QMap<load_res, uchar> &index, TabletCanvas *canvas);
    load_res load(const QString &, const bool clear, TabletCanvas *canvas);
    /* it load from a zip_t file all the pdf for the current copybook */
    load_res load(zip_t *filezip, zip_file_t *file, TabletCanvas *);
    load_res load_from_row(const QByteArray &, const bool clear,
                           const bool FirstLoad, const uchar IndexPdf,
                           TabletCanvas *canvas);

    void resizing(TabletCanvas *canvas, const uint lenPdf);

    load_res save(zip_t *filezip, const QStringList &path, const QString &path_writernote_file);
    load_res save(zip_t *filezip, const QString &path, const QString &path_writernote_file);

    load_res save_metadata(zip_source_t *file);

    inline void draw(QPainter &painter,
                     const uint pwidth,
                     const int rend_width,
                     const int rend_heigth,
                     double delta,
                     const bool IsExportingPdf) const{
        Q_UNUSED(pwidth);

        static uint i, k, len_img;
        static double x;
        static QRectF size;
        static const Pdf *pdf;

        const uint len = this->m_image.length();
        const double y = (IsExportingPdf) ? rend_heigth * delta : m_data->datatouch->currentHeight()*delta;
        x = 0;

        if(!len)
            return;

        if(IsExportingPdf){
            size = QRectF(-m_image.first().topLeft.x()*delta,
                          -m_image.first().topLeft.y()*delta,
                          double(rend_width)*delta,
                          double(rend_heigth)*delta);

            x = rend_width*delta;
        }else{
            size = this->m_data->datatouch->pos_first_page();

            size = QRectF(size.topLeft().x()*delta,
                          size.topLeft().y()*delta,
                          size.width()*delta,
                          size.height()*delta);
        }

        for(i=0; i < len; ++i){
            pdf = &this->m_image.at(i);
            len_img = pdf->img.length();
            for(k=0; k < len_img; ++k){
                qDebug() << "void draw " << size.topLeft() << pdf->topLeft;
                fromimage::draw(painter, size, pdf->img.at(k));

                if(IsExportingPdf)
                    size.setY(size.y() + y + pdf->topLeft.y()*delta);
                else
                    size.setY(size.y() + y);

                size.setHeight(y);

                if(IsExportingPdf)
                    size.setWidth(x);


            }
        }
    }

    uchar insert_pdf(QString &pos,
                     const PointSettable *point);

    void addPdf(QString &pos,
                const PointSettable *point,
                const QString &path_writernote,
                TabletCanvas *canvas);

    uint resolution = 500;//72
private:
    void adjast(const uchar indexPdf);
    load_res load_metadata(zip_file_t *file);

    /*
     * this function only append a pdf to
     * the list
    */
    void init_FirstLoad();
};

#endif // PDFSUPPORT
#endif // FROMPDF_H
