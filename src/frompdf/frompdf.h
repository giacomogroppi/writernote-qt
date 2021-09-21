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
    /* indice la parte in alto a sinistra della prima immagine */
    QPointF topLeft;

    /* indica la parte in basso a destra della prima immagine */
    QPointF bottomRigth;
    QList<immagine_s> img;
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
                     const bool IsExportingPdf){
        Q_UNUSED(pwidth);

        int i, k;
        double y, x = 0;
        QRectF size = this->m_data->datatouch->pos_first_page();

        if(IsExportingPdf){
            size = QRectF(0,
                          0,
                          double(rend_width)*delta,
                          double(rend_heigth)*delta);
            y = rend_heigth*delta;
            x = rend_width*delta;
        }else{
            size = QRectF(size.topLeft().x()*delta,
                          size.topLeft().y()*delta,
                          size.bottomRight().x()*delta,
                          size.bottomRight().y()*delta);
            y = this->m_data->datatouch->currentHeight()*delta;
        }

        for(i=0; i<this->m_image.length(); ++i){
            const Pdf &pdf = this->m_image.at(i);
            for(k=0; k<pdf.img.length(); ++k){
                fromimage::draw(painter, size, pdf.img.at(i).immagini);

                qDebug() << size << rend_heigth << rend_width << x << y;

                size = QRectF(size.topLeft().x(),
                              size.topLeft().y() + y,
                              size.bottomRight().x() + x,
                              size.bottomRight().y() + y);

                //size.adjust(0, y, 0, 0);
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
private:
    void adjast(const uchar indexPdf);
    load_res load_metadata(zip_file_t *file);

    uint resolution = 1000;//72

    /*
     * this function only append a pdf to
     * the list
    */
    void init_FirstLoad();
};
#endif // PDFSUPPORT
#endif // FROMPDF_H
