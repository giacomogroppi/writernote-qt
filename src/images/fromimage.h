#pragma once

#include "core/WZipReaderSingle.h"
#include "core/WZipWriter.h"
#include "zip.h"
#include <QList>
#include "utils/common_error_definition.h"
#include "touch/datastruct/datastruct.h"
#include <QPointF>
#include <QImage>
#include <QPainter>
#include "core/WZip.h"

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

    static inline QByteArray getName(const unsigned i);

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

    [[nodiscard]] int addImage(QString &pos, const PointSettable *point, const QString &writernote_file);

    fromimage(Document *doc){ this->doc = doc; }

#ifdef ALL_VERSION
    fromimage::load_res load(zip_t *filezip, zip_file_t *file);
#endif // ALL_VERSION
    fromimage::load_res load(WZipReaderSingle &zip);

    fromimage::load_res save(WZipWriter &file, const QStringList &path) const;
    fromimage::load_res save(WZipWriter &file, const QString &path) const;
    fromimage::load_res save_metadata(WZipWriterSingle &writer);

    void move(const QPointF &translation);

    void moveImage(const QList<int> &index, const QPointF &translation);

    static  void draw(QPainter &painter, const QRectF &rect, const QImage &img);
    static  void draw(QPainter &painter, const immagine_s &img);
    static  void draw(QPainter &painter, const QList<immagine_s> &list);
            void draw(QPainter &painter) const;

    void reset();
    size_t get_size_file() const;

private:
    load_res get_img_bytearray(QByteArray &arr, const QString &path) const;
#ifdef ALL_VERSION
    load_res load_metadata(zip_file_t *file);
#endif
    load_res load_metadata(WZipReaderSingle &reader);

    load_res load_single(const QByteArray &arr,
                         struct immagine_s &img);
    load_res load_multiple(const QList<QByteArray> &arr);
};

inline QByteArray fromimage::getName(const unsigned i)
{
    const auto tmp = SUFFIX_IMG + QString::number(uint(i));
    return tmp.toUtf8();
}

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
    int i = list.length() - 1;

    for(i--; i >= 0; i--){
        const struct immagine_s &img = list.at(i);

        fromimage::draw(painter, img);
    }
}

inline void fromimage::draw(QPainter &painter) const
{
    return fromimage::draw(painter, this->m_img);
}

inline void fromimage::reset()
{
    m_img.clear();
}
