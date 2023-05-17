#pragma once

#include "core/WZipReaderSingle.h"
#include "core/WZipWriter.h"
#include "zip.h"
#include "utils/common_error_definition.h"
#include "touch/dataTouch/datastruct/DataStruct.h"
#include "core/WZip.h"
#include "core/WImage.h"

#define DELTA_POINT 200
#define SUFFIX_IMG "_img_"

class Document;
struct immagine_s{
    WImage immagini;
    PointF i;
    PointF f;
};

class fromimage{
private:
    WListFast<struct immagine_s> m_img;
    WListFast<WString> get_name_img();
    unsigned insert_image(const WString &__pos, const PointSettable *point, struct immagine_s &img);

    static inline WByteArray getName_img(unsigned i);
public:
    static void copy_img(const fromimage &src, fromimage &dest);
    friend class SquareMethod;

    enum load_res_img: unsigned char {
        ok,
        error,
        err_meta_data,
        err_image_not_valid
    };

    [[nodiscard]] int addImage(const WString &pos, const PointSettable *point, const WString &writernote_file);

    explicit fromimage();

    [[nodiscard]] int length_img() const { return m_img.size(); };
    [[nodiscard]] fromimage::load_res_img load_img(WZipReaderSingle &zip, int len);
    fromimage::load_res_img save_img(WZipWriter &file, const WListFast<WString> &path) const;
    fromimage::load_res_img save_img(WZipWriter &file, const WString &path) const;
    fromimage::load_res_img save_metadata_img(WZipWriterSingle &writer);

    void move_img(const PointF &translation);

    void moveImage(const WListFast<int> &index, const PointF &translation);

    static  void draw_img(WPainter &painter, const RectF &rect, const WImage &img);
    static  void draw_img(WPainter &painter, const immagine_s &img);
    static  void draw_img(WPainter &painter, const WListFast<immagine_s> &list);

    void draw_img(WPainter &painter) const;

    void reset_img();
    [[nodiscard]] size_t get_size_file_img() const;

private:
    static load_res_img get_img_bytearray(WByteArray &arr, const WString &path) ;
    load_res_img load_metadata_img(WZipReaderSingle &reader, int len);
    load_res_img load_single_img(const WByteArray &arr, struct immagine_s &img);
    load_res_img load_multiple_img(const WListFast<WByteArray> &arr);
};

inline WByteArray fromimage::getName_img(const unsigned i)
{
    const auto tmp = WString(SUFFIX_IMG) + WString::number(i);
    return tmp.toUtf8();
}

inline void fromimage::copy_img(const fromimage &src, fromimage &dest)
{
    dest.m_img = src.m_img;
}

inline fromimage::fromimage()
{
}

inline void fromimage::move_img(const PointF &translation)
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

inline void fromimage::moveImage(const WListFast<int> &index, const PointF &translation)
{
    for(const auto currentIndex : std::as_const(index)){
        this->m_img.operator[](currentIndex).i += translation;
        this->m_img.operator[](currentIndex).f += translation;
    }
}

inline void fromimage::draw_img(WPainter &painter, const RectF &rect, const WImage &img)
{
    const RectF draw = img.rect();

    painter.drawImage(rect, img, draw);
}

inline void fromimage::draw_img(WPainter &painter, const immagine_s &img)
{
    int check;

    check = (img.f.y() < 0.) or (img.f.x() < 0.);

    if(check)
        return;

    fromimage::draw_img(painter, RectF(img.i, img.f), img.immagini);
}

force_inline void fromimage::draw_img(WPainter &painter, const WListFast<immagine_s> &list)
{
    for (const auto &img : std::as_const(list)) {
        fromimage::draw_img(painter, img);
    }
}

force_inline void fromimage::draw_img(WPainter &painter) const
{
    return fromimage::draw_img(painter, this->m_img);
}

inline void fromimage::reset_img()
{
    m_img.clear();
}
