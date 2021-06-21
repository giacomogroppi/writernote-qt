#ifndef FROMPDF_H
#define FROMPDF_H

class currenttitle_class;

#include <QString>
#include <QList>
#include <QImage>
#include <QPainter>

#include "poppler-qt5.h"
#define IMG_PDF_HEIGHT 292
#define IMG_PDF_WIDTH 210

class frompdf
{
private:
    currenttitle_class *m_data;

    QList<QImage> images;


    Poppler::Document *doc = nullptr;
    QList<QImage> m_image;

public:
    static bool isvalid(QString &pos);

    frompdf(currenttitle_class *);

    enum load_res: uchar{
        ok,
        not_valid_pdf,
        not_valid_page
    };

    load_res load(const QString &);

    inline void draw(QPainter &painter,
                     const uint pwidth,
                     const int rend_width,
                     const int rend_heigth){
        uint i, len;
        QRect rect_area;

        len = images.length();

        for(i=0; i<len; ++i){
            rect_area = QRect(pwidth*i,
                              (IMG_PDF_HEIGHT/IMG_PDF_WIDTH)*pwidth*i,
                              pwidth*(i+1),
                              (IMG_PDF_HEIGHT/IMG_PDF_WIDTH)*pwidth*(i+1));

            if(rect_area.bottomRight().x() < 0)
                continue;
            if(rect_area.topLeft().x() > rend_width)
                continue;

            if(rect_area.bottomRight().y() < 0)
                continue;
            if(rect_area.topLeft().y() > rend_heigth)
                continue;

            painter.drawImage(rect_area, m_image.at(i));
        }
    }

};

#endif // FROMPDF_H
