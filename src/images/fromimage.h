#pragma once

#include "FileContainer/WZipReaderSingle.h"
#include "FileContainer/WZipWriter.h"
#include "utils/common_error_definition.h"
#include "touch/dataTouch/datastruct/DataStruct.h"
#include "FileContainer/WZip.h"
#include "core/Image/WImage.h"
#include "ImageDrawable.h"

#define DELTA_POINT 200
#define SUFFIX_IMG "_img_"

class Document;

class ImageContainerDrawable
{
private:
    WListFast<ImageDrawable> m_img;
    WListFast<WString> get_name_img();
    static unsigned insert_image(const WString &__pos, const PointSettable *point, ImageDrawable &img);

    static inline WByteArray getName_img(unsigned i);
public:
    ImageContainerDrawable (const ImageContainerDrawable &other) noexcept;
    ImageContainerDrawable (ImageContainerDrawable &&other) noexcept;

    static void copy_img(const ImageContainerDrawable &src, ImageContainerDrawable &dest);
    friend class SquareMethod;

    enum load_res_img: unsigned char {
        ok,
        error,
        err_meta_data,
        err_image_not_valid
    };

    [[nodiscard]]
    auto addImage(const WString &pos, const PointSettable *point, const WString &writernote_file) -> int;

    explicit ImageContainerDrawable();

    [[nodiscard]] int lengthImage() const { return m_img.size(); };
    [[nodiscard]] ImageContainerDrawable::load_res_img loadImage(WZipReaderSingle &zip, int len);
    ImageContainerDrawable::load_res_img saveImage(WZipWriter &file, const WListFast<WString> &path) const;
    ImageContainerDrawable::load_res_img saveImage(WZipWriter &file, const WString &path) const;
    ImageContainerDrawable::load_res_img saveMetadataImage(WZipWriterSingle &writer);

    void moveImage(const PointF &translation);

    void moveImage(const WListFast<int> &index, const PointF &translation);

    static  void drawImage(WPainter &painter, const RectF &rect, const WImage &img);
    static  void drawImage(WPainter &painter, const ImageDrawable &img);
    static  void drawImage(WPainter &painter, const WListFast<ImageDrawable> &list);

    void drawImage(WPainter &painter) const;

    void reset_img();
    [[nodiscard]]
    auto getSizeFileImage() const -> size_t;

    auto operator=(const ImageContainerDrawable &other) noexcept -> ImageContainerDrawable &;
    auto operator=(ImageContainerDrawable &&other) noexcept -> ImageContainerDrawable &;

private:
    static load_res_img getImageRawData(WByteArray &arr, const WString &path) ;
    load_res_img loadMetadataImage(WZipReaderSingle &reader, int len);
    static load_res_img loadSingleImage(const WByteArray &arr, ImageDrawable &img);
    load_res_img loadMultipleImage(const WListFast<WByteArray> &arr);

protected:
    /**
     * \return &lt 0 in case of error
     * */
    static auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<int, ImageContainerDrawable>;

    /**
     * \resurn &lt 0 in case of error
     * */
    static auto write (WritableAbstract &writable, const ImageContainerDrawable &source) -> int;
};

inline WByteArray ImageContainerDrawable::getName_img(const unsigned i)
{
    const auto tmp = WString(SUFFIX_IMG) + WString::number(i);
    return tmp.toUtf8();
}

inline void ImageContainerDrawable::copy_img(const ImageContainerDrawable &src, ImageContainerDrawable &dest)
{
    dest.m_img = src.m_img;
}

inline ImageContainerDrawable::ImageContainerDrawable() = default;

inline void ImageContainerDrawable::moveImage(const PointF &translation)
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

inline void ImageContainerDrawable::moveImage(const WListFast<int> &index, const PointF &translation)
{
    for (const auto currentIndex : std::as_const(index)) {
        this->m_img.operator[](currentIndex).i += translation;
        this->m_img.operator[](currentIndex).f += translation;
    }
}

inline void ImageContainerDrawable::drawImage(WPainter &painter, const RectF &rect, const WImage &img)
{
    const RectF draw = img.rect();

    painter.drawImage(rect, img, draw);
}

inline void ImageContainerDrawable::drawImage(WPainter &painter, const ImageDrawable &img)
{
    int check;

    check = (img.f.y() < 0.) or (img.f.x() < 0.);

    if(check)
        return;

    ImageContainerDrawable::drawImage(painter, RectF(img.i, img.f), img.immagini);
}

force_inline void ImageContainerDrawable::drawImage(WPainter &painter, const WListFast<ImageDrawable> &list)
{
    for (const auto &img : std::as_const(list)) {
        ImageContainerDrawable::drawImage(painter, img);
    }
}

force_inline void ImageContainerDrawable::drawImage(WPainter &painter) const
{
    return ImageContainerDrawable::drawImage(painter, this->m_img);
}

inline void ImageContainerDrawable::reset_img()
{
    m_img.clear();
}
