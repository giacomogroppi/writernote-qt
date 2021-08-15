#ifndef FROMIMAGE_H
#define FROMIMAGE_H

#include "zip.h"
#include <QList>
#include "../utils/common_error_definition.h"
#include "../touch/datastruct/datastruct.h"
#include <QPointF>
#include <QImage>
#include <QPainter>

#define delta_point 200
#define SUFFIX_IMG "_img_"

class Document;
struct immagine_S{
    QImage immagini;
    QPointF i;
    QPointF f;
};

class fromimage{
private:
    Document *doc;

    QList<struct immagine_S> m_img;
    inline QStringList get_name_img();

    static inline QString getNameNoCopy(const uint i){
        return SUFFIX_IMG + QString::number(uint(i));
    }

    static inline QString getName(const QString &p, const uint i){
        return p + fromimage::getNameNoCopy(i);
    }

    immagine_S *insert_image(const char *__pos,
                      struct PointSettable * point);


public:
    enum load_res: uchar{
        ok,
        error
    };

    void addImage(Document *m_currenttitle,
                         const char *__pos,
                         struct PointSettable * point);

    inline fromimage(Document *doc){
        this->doc = doc;
    }

    fromimage::load_res load(zip_t *file, const bool clear);
    uchar save(zip_t *file) const;

    inline void move(const QPointF &translation){

    }

    inline void draw(QPainter &painter,
                     const uint pwidth,
                     const int rend_width,
                     const int rend_heigth) const{
        uint i, len;
        QRectF rect;

        len = this->m_img.length();
        for(i=0; i<len; ++i){
            const struct immagine_S &img = m_img.at(i);
            if(img.f.y() < 0)
                continue;
            if(img.f.x() < 0)
                continue;
            if(img.i.y() > rend_width)
                continue;
            if(img.i.x() > rend_heigth)
                continue;

            rect = QRectF(img.i, img.f);

            painter.drawImage(rect, img.immagini, rect);

        }
    }

    void reset(){
        m_img.clear();
    }

private:
    static load_res load_single(const QByteArray &arr, struct immagine_S &img);
    static load_res load_multiple(const QList<QByteArray> &arr, QList<struct immagine_S> &);
};

#endif // FROMIMAGE_H
