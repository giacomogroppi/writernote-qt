#ifndef FROMIMAGE_H
#define FROMIMAGE_H

#include "zip.h"
#include <QList>
#include "../utils/common_error_definition.h"
#include "../touch/datastruct/datastruct.h"
#include <QPointF>
#include <QImage>
#include <QPainter>

#define DELTA_POINT 200
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

    uchar insert_image(QString &__pos,
                      const PointSettable *point,
                      struct immagine_S &img);


public:
    enum load_res: uchar{
        ok,
        error,
        err_meta_data
    };

    void addImage(QString &__pos,
                  const PointSettable *point,
                  const QString &writernote_file);

    inline fromimage(Document *doc){
        this->doc = doc;
    }

    fromimage::load_res load(zip_t *filezip, zip_file_t *file);

    fromimage::load_res save(zip_t *file, const QStringList &path, const QString &path_writernote_file) const;
    fromimage::load_res save(zip_t *file, const QString &path, const QString &path_writernote_file) const;

    inline void move(const QPointF &translation){
        uint i, len;
        struct immagine_S *img;
        double x, y;

        x = translation.x();
        y = translation.y();

        len = this->m_img.length();

        for(i=0; i<len; ++i){
            img = (immagine_S *)&m_img.at(i);

            img->f.setX(img->f.x() + x);
            img->f.setY(img->f.y() + y);

            img->i.setX(img->i.x() + x);
            img->i.setY(img->i.y() + y);
        }
    }

    inline void draw(QPainter &painter,
                     const uint pwidth,
                     const int rend_width,
                     const int rend_heigth) const{
        Q_UNUSED(pwidth);
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
    load_res load_meta(zip_file_t *file);

    load_res load_single(const QByteArray &arr,
                         struct immagine_S &img);
    load_res load_multiple(const QList<QByteArray> &arr);
};

#endif // FROMIMAGE_H
