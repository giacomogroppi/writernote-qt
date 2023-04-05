#pragma once

#include "core/WZipReaderSingle.h"
#include "core/WZipWriter.h"
#include "zip.h"
#include <QList>
#include "utils/common_error_definition.h"
#include "touch/dataTouch/datastruct/DataStruct.h"
#include <QPointF>
#include <QPainter>
#include "core/WZip.h"
#include "core/WImage.h"

#define DELTA_POINT 200
#define SUFFIX_IMG "_img_"

class Document;
struct immagine_s{
    WPixmap immagini;
    QPointF i;
    QPointF f;
};

class fromimage{
private:
    QList<struct immagine_s> m_img;
    QList<QString> get_name_img();
    unsigned insert_image(const QString &__pos, const PointSettable *point, struct immagine_s &img);

    static inline QByteArray getName_img(unsigned i);
public:
    static void copy_img(const fromimage &src, fromimage &dest);
    friend class SquareMethod;

    enum load_res_img: uchar{
        ok,
        error,
        err_meta_data,
        err_image_not_valid
    };

    [[nodiscard]] int addImage(const QString &pos, const PointSettable *point, const QString &writernote_file);

    explicit fromimage();

    [[nodiscard]] int length_img() const { return m_img.length(); };
    [[nodiscard]] fromimage::load_res_img load_img(WZipReaderSingle &zip, int len);
    fromimage::load_res_img save_img(WZipWriter &file, const QList<QString> &path) const;
    fromimage::load_res_img save_img(WZipWriter &file, const QString &path) const;
    fromimage::load_res_img save_metadata_img(WZipWriterSingle &writer);

    void move_img(const QPointF &translation);

    void moveImage(const QList<int> &index, const QPointF &translation);

    static  void draw_img(QPainter &painter, const QRectF &rect, const WPixmap &img);
    static  void draw_img(QPainter &painter, const immagine_s &img);
    static  void draw_img(QPainter &painter, const QList<immagine_s> &list);

    void draw_img(QPainter &painter) const;

    void reset_img();
    [[nodiscard]] size_t get_size_file_img() const;

private:
    static load_res_img get_img_bytearray(QByteArray &arr, const QString &path) ;
    load_res_img load_metadata_img(WZipReaderSingle &reader, int len);
    load_res_img load_single_img(const QByteArray &arr, struct immagine_s &img);
    load_res_img load_multiple_img(const QList<QByteArray> &arr);
};

inline QByteArray fromimage::getName_img(const unsigned i)
{
    const auto tmp = SUFFIX_IMG + QString::number(uint(i));
    return tmp.toUtf8();
}

inline void fromimage::copy_img(const fromimage &src, fromimage &dest)
{
    dest.m_img = src.m_img;
}

inline fromimage::fromimage()
{
}

inline void fromimage::move_img(const QPointF &translation)
{
    double x, y;

    x = translation.x();
    y = translation.y();

    for(auto &img : m_img){
        img.f.setX(img.f.x() + x);
        img.f.setY(img.f.y() + y);

        img.i.setX(img.i.x() + x);
        img.i.setY(img.i.y() + y);
    }
}

inline void fromimage::moveImage(const QList<int> &index, const QPointF &translation)
{
    for(const auto currentIndex : qAsConst(index)){
        this->m_img.operator[](currentIndex).i += translation;
        this->m_img.operator[](currentIndex).f += translation;
    }
}

inline void fromimage::draw_img(QPainter &painter, const QRectF &rect, const WPixmap &img)
{
    const QRectF draw = img.rect();

    painter.drawPixmap(rect, img, draw);
}

inline void fromimage::draw_img(QPainter &painter, const immagine_s &img)
{
    int check;

    check = (img.f.y() < 0.) or (img.f.x() < 0.);

    if(check)
        return;

    fromimage::draw_img(painter, QRectF(img.i, img.f), img.immagini);
}

force_inline void fromimage::draw_img(QPainter &painter, const QList<immagine_s> &list)
{
    for (const auto &img : qAsConst(list)) {
        fromimage::draw_img(painter, img);
    }
}

force_inline void fromimage::draw_img(QPainter &painter) const
{
    return fromimage::draw_img(painter, this->m_img);
}

inline void fromimage::reset_img()
{
    m_img.clear();
}
