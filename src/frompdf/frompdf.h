#ifndef FROMPDF_H
#define FROMPDF_H

class currenttitle_class;

#include <QString>
#include <QList>
#include <QImage>
#include <QPainter>

#include "poppler-qt5.h"

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
                     const uint height,
                     const uint width){
        uint i, len;
        QRect rect_area(0, 0, width, height);

        len = images.length();

        for(i=0; i<len; ++i){
            painter.drawImage(rect_area, m_image.at(i));
        }
    }

};

#endif // FROMPDF_H
