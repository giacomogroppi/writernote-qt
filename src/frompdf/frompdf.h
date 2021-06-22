#ifndef FROMPDF_H
#define FROMPDF_H

class currenttitle_class;

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

public:    
    static bool isvalid(QString &pos);

    void traslation(const double x,
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

    /* return true if all load correctly */
    bool load(const QStringList &path, QMap<load_res, uchar> &index);
    load_res load(const QString &, const bool clear);

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
