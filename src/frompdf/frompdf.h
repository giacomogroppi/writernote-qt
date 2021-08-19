#ifndef FROMPDF_H
#define FROMPDF_H

class Document;

#include "../touch/datastruct/datastruct.h"
#include "zip.h"
#include <QString>
#include <QList>
#include <QImage>
#include <QMap>
#include <QPainter>

#include "poppler-qt5.h"
#define IMG_PDF_HEIGHT 292
#define IMG_PDF_WIDTH 210

#define SUFFIX_PDF "_pdf_"

class frompdf
{
private:
    Document *m_data;

    Poppler::Document *doc = nullptr;
    QList<QImage> m_image;

    struct s_translation{
        double y1;
        double x1;
        double y2;
        double x2;
    }m_translation;

    static inline QString getNameNoCopy(const uint i){
        return SUFFIX_PDF + QString::number(uint(i));
    }

    static inline QString getName(const QString &p, const uint i){
        return p + frompdf::getNameNoCopy(i);
    }

public:    
    static bool isvalid(QString &pos);

    void translation(const QPointF &point){
        translation(point.x(), point.y());
    }
    void translation(const double x, const double y);

    void getPosition(QPointF &topLeft, QPointF &bottomRight){
        topLeft = QPointF(m_translation.x1, m_translation.y1);
        bottomRight = QPointF(m_translation.x2, m_translation.y2);
    }

    frompdf(Document *doc);

    enum load_res: uchar{
        ok,
        not_valid_pdf,
        not_valid_page,
        no_metadata
    };

    inline void reset(){
        m_image.clear();
    }

    QStringList get_name_pdf();

    /* return true if all load correctly */
    bool load(const QStringList &path, QMap<load_res, uchar> &index);
    load_res load(const QString &, const bool clear);
    /* it load from a zip_t file all the pdf for the current copybook */
    load_res load(zip_t *filezip, zip_file_t *file);
    load_res load_from_row(const QByteArray &, const bool clear);

    load_res save(zip_t *filezip, const QStringList &path, const QString &path_writernote_file);
    load_res save(zip_t *filezip, const QString &path, const QString &path_writernote_file);

    load_res save_metadata(zip_source_t *file);

    inline void draw(QPainter &painter,
                     const uint pwidth,
                     const int rend_width,
                     const int rend_heigth){
        uint i, len;
        QRect rect_area;
        uchar check;
        size_t height;

        len = m_image.length();
        height = 0;

        for(i=0; i<len; ++i){
            const QImage &img = m_image.at(i);

            rect_area = QRect(pwidth*i,
                              (IMG_PDF_HEIGHT/IMG_PDF_WIDTH)*pwidth*i,
                              pwidth*(i+1),
                              (IMG_PDF_HEIGHT/IMG_PDF_WIDTH)*pwidth*(i+1));

            check = (m_translation.y2 < (double)0) +
                (m_translation.x2 < (double)0) +
                (m_translation.y1 > (double)rend_width) +
                (m_translation.x1 > (double)rend_heigth);

            if(check)
                continue;

            /*
             * we need to add all the
             * height of all the image we draw
             * before
            */
            height += img.height();

            painter.drawImage(rect_area, img);
        }
    }

    uchar insert_pdf(QString &pos,
                     const PointSettable *point);

    void addPdf(QString &pos,
                      const PointSettable *point,
                      const QString &path_writernote);
private:
    load_res load_metadata(zip_file_t *file);
};

#endif // FROMPDF_H
