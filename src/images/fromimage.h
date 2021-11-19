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
struct immagine_s{
    QImage immagini;
    QPointF i;
    QPointF f;
};

class fromimage{
private:
    Document *doc;

    QList<struct immagine_s> m_img;
    QStringList get_name_img();
    static QStringList get_name_img(const Document &doc);

    static inline QString getName(const uint i){ return SUFFIX_IMG + QString::number(uint(i)); }

    uchar insert_image(QString &__pos, const PointSettable *point, struct immagine_s &img);


public:
    static void copy(const fromimage &src, fromimage &dest);
    friend class square;

    enum load_res: uchar{
        ok,
        error,
        err_meta_data,
        err_image_not_valid
    };

    void addImage(QString &__pos,
                  const PointSettable *point,
                  const QString &writernote_file);

    fromimage(Document *doc){ this->doc = doc; }

    fromimage::load_res load(zip_t *filezip, zip_file_t *file);

    fromimage::load_res save(zip_t *file, const QStringList &path, const QString &path_writernote_file) const;
    fromimage::load_res save(zip_t *file, const QString &path, const QString &path_writernote_file) const;
    fromimage::load_res save_metadata(zip_source_t *file);

    void move(const QPointF &translation);

    void moveImage(const QList<int> &index, const QPointF &translation);

    static  void draw(QPainter &painter, const QRectF &rect, const QImage &img);
    static  void draw(QPainter &painter, const immagine_s &img);
    static  void draw(QPainter &painter, const QList<immagine_s> &list);
            void draw(QPainter &painter) const;

    void reset(){ m_img.clear(); }

private:
    load_res get_img_bytearray(QByteArray &arr, const QString &path) const;
    load_res load_metadata(zip_file_t *file);

    load_res load_single(const QByteArray &arr,
                         struct immagine_s &img);
    load_res load_multiple(const QList<QByteArray> &arr);
};

inline void fromimage::copy(const fromimage &src, fromimage &dest)
{
    dest.m_img = src.m_img;
}

inline void fromimage::move(const QPointF &translation)
{
    uint i, len;
    struct immagine_s *img;
    double x, y;

    x = translation.x();
    y = translation.y();

    len = this->m_img.length();

    for(i=0; i<len; ++i){
        img = (immagine_s *)&m_img.operator[](i);

        img->f.setX(img->f.x() + x);
        img->f.setY(img->f.y() + y);

        img->i.setX(img->i.x() + x);
        img->i.setY(img->i.y() + y);
    }

}

inline void fromimage::moveImage(const QList<int> &index, const QPointF &translation)
{
    uint i, lenght;

    lenght = index.length();
    for(i=0; i<lenght; ++i){
        const int currentIndex = index.at(i);
        this->m_img.operator[](currentIndex).i += translation;
        this->m_img.operator[](currentIndex).f += translation;
    }
}

inline void fromimage::draw(QPainter &painter, const QRectF &rect, const QImage &img)
{
    const QRectF draw = img.rect();

    painter.drawImage(rect, img, draw);
}

inline void fromimage::draw(QPainter &painter, const immagine_s &img)
{
    uchar check;

    check = (img.f.y() < double(0)) +
            (img.f.x() < double(0));

    if(check)
        return;

    fromimage::draw(painter, QRectF(img.i, img.f), img.immagini);

}

inline void fromimage::draw(QPainter &painter, const QList<immagine_s> &list)
{
    uint i;

    for(i=0; i<(uint)list.length(); ++i){
        const struct immagine_s &img = list.at(i);

        fromimage::draw(painter, img);
    }
}

inline void fromimage::draw(QPainter &painter) const
{
    return fromimage::draw(painter, this->m_img);
}

#endif // FROMIMAGE_H
