#ifndef FROMPDF_H
#define FROMPDF_H

class currenttitle_class;

#include "zip.h"
#include <QString>
#include <QList>
#include <QImage>
#include <QMap>
#include <QPainter>

#include "poppler-qt5.h"
#define IMG_PDF_HEIGHT 292
#define IMG_PDF_WIDTH 210

class frompdf
{
private:
    currenttitle_class *m_data;

    Poppler::Document *doc = nullptr;
    QList<QImage> m_image;

    struct translation{
        double y;
        double x;
    }m_translation;

    static inline QString getName(const QString &p, const uint i){
        return p + "_pdf_" + QString::number(uint(i));
    }

public:    
    static bool isvalid(QString &pos);

    void translation(const QPointF &point){
        translation(point.x(), point.y());
    }
    void translation(const double x,
                    const double y){
        m_translation.x += x;
        m_translation.y += y;
    }

    void getPosition(double &x, double &y){
        x = m_translation.x;
        y = m_translation.y;
    }

    frompdf(currenttitle_class *);

    enum load_res: uchar{
        ok,
        not_valid_pdf,
        not_valid_page
    };

    inline void reset(){
        m_image.clear();
    }

    QStringList get_name_pdf();

    /* return true if all load correctly */
    bool load(const QStringList &path, QMap<load_res, uchar> &index);

    load_res load(const QString &, const bool clear);

    /* it load from a zip_t file all the pdf for the current copybook */
    load_res load(zip_t *, const bool clear);

    load_res load_from_row(const QByteArray &, const bool clear);

    inline void draw(QPainter &painter,
                     const uint pwidth,
                     const int rend_width,
                     const int rend_heigth){
        uint i, len;
        QRect rect_area;

        len = m_image.length();

        for(i=0; i<len; ++i){
            rect_area = QRect(pwidth*i,
                              (IMG_PDF_HEIGHT/IMG_PDF_WIDTH)*pwidth*i,
                              pwidth*(i+1),
                              (IMG_PDF_HEIGHT/IMG_PDF_WIDTH)*pwidth*(i+1));

            if(rect_area.bottomRight().x()+m_translation.x < 0)
                continue;
            if(rect_area.topLeft().x()+m_translation.x > rend_width)
                continue;

            if(rect_area.bottomRight().y()+m_translation.y < 0)
                continue;
            if(rect_area.topLeft().y()+m_translation.y > rend_heigth)
                continue;

            painter.drawImage(rect_area, m_image.at(i));
        }
    }

};

#endif // FROMPDF_H
