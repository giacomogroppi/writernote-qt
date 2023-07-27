#pragma once

#ifdef USE_QT

#include "core/ByteArray/WByteArray.h"
#include "WImagePrivateCommon.h"
#include "core/RectF.h"
#include "core/String/WString.h"
#include "core/WRgb.h"
#include "core/Point.h"
#include <iostream>
#include <QBuffer>
#include <QImage>

class WImage : public QImage
{
public:
    explicit WImage(const WString &path) noexcept;
    WImage (WImage &&other) noexcept;
    WImage (const WImage &other) noexcept;

    explicit WImage (QImage img);

    explicit WImage(const std::string &fileName, const char *format = nullptr);
    explicit WImage(int page, bool consideringResolution);
    WImage(int width, int height, WImageType format);
    WImage() = default;

    [[nodiscard]]
    auto get_size_in_file() const -> size_t;
    
    [[nodiscard]]
    auto save_and_size(WByteArray &arr) const -> size_t;

    bool loadFromData(const WByteArray &data, const char *formact);

    auto height() const -> int;
    auto width() const -> int;

    auto rect() const -> RectF;

    bool isNull() const;

    static auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<int, WImage>;
    static auto write (WritableAbstract &writable, const WImage &source) -> int;

    auto operator==(const WImage &other) const -> bool;
    auto operator=(WImage &&other) noexcept -> WImage &;

    auto getRawDataPNG() const -> WByteArray;

    WImage &operator=(const QImage &other);

    friend class WPainter;

    WRgb pixel(const WPoint &point) const;
};

inline auto WImage::loadFromData(const WByteArray &data, const char *formact) -> bool
{
    return QImage::loadFromData(data, formact);
}

inline auto WImage::isNull() const -> bool
{
    return QImage::isNull();
}

inline auto WImage::rect() const -> RectF
{
    const auto qtRect = QImage::rect();
    const auto tl = qtRect.topLeft();
    const auto rb = qtRect.bottomRight();

    return RectF {
            (double) tl.x(),
            (double) tl.y(),
            (double) rb.x(),
            (double) rb.y()
    };
}

inline auto WImage::operator==(const WImage &other) const -> bool
{
    return QImage::operator==(other);
}

inline auto WImage::width() const -> int
{
    return QImage::width();
}

inline auto WImage::height() const -> int
{
    return QImage::height();
}

inline auto WImage::operator=(const QImage &other) -> WImage&
{
    if (this == &other)
        return *this;
    QImage::operator==(other);
    return *this;
}

inline WImage::WImage(QImage img)
    : QImage(std::move(img))
{

}

inline auto WImage::pixel(const WPoint &point) const -> WRgb
{
    return QImage::pixel(point.x(), point.y());
}

inline WImage::WImage(const WString &path) noexcept
    : QImage(path)
{
}

inline WImage::WImage(WImage &&other) noexcept
    : QImage(std::move(other))
{

}

inline auto WImage::operator=(WImage &&other) noexcept -> WImage &
{
    if (this == &other)
        return *this;
    QImage::operator=(std::move(other));
    return *this;
}

inline auto WImage::getRawDataPNG() const -> WByteArray
{
    QByteArray arr;
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    QImage::save(&buffer, "PNG");
    buffer.close();
    return {std::move(arr)};
}

inline WImage::WImage(const WImage &other) noexcept = default;

#endif // USE_QT