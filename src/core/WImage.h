#pragma once

#ifdef USE_QT
#include <QImage>
#include <QBuffer>
#include "core/ByteArray/WByteArray.h"
#else
# include "core/ByteArray/WByteArray.h"
#endif

#include "core/ByteArray/WByteArray.h"
#include "RectF.h"
#include "String/WString.h"
#include "WRgb.h"
#include "Point.h"
#include <iostream>

enum WImageType {
    png
};

class WImage
#ifdef USE_QT
        : public QImage
#endif
        {
private:
#ifdef Q_OS_IOS
    /**
     * this is a NSImage object
    */
    void *d;
#endif
public:
    explicit WImage(const WString &path) noexcept;
    WImage (WImage &&other) noexcept;
    WImage (const WImage &other) noexcept;

#ifdef USE_QT
    explicit WImage (QImage img);
#endif // USE_QT
    explicit WImage(const std::string &fileName, const char *format = nullptr);
    explicit WImage(int page, bool consideringResolution);
    WImage(int width, int height, WImageType format);
    WImage() = default;

    [[nodiscard]] size_t get_size_in_file() const;
    [[nodiscard]] size_t save_and_size(WByteArray &arr) const;

    bool loadFromData(const WByteArray &data, const char *formact);

    int height() const;
    int width() const;

    RectF rect() const;

    bool isNull() const;

    bool operator==(const WImage &other) const;
    WImage &operator=(WImage &&other) noexcept;

#ifdef USE_QT
    WImage &operator=(const QImage &other);
#endif // USE_QT

    friend class WPainter;

    WRgb pixel(const WPoint &point) const;
};

#ifdef USE_QT

inline bool WImage::loadFromData(const WByteArray &data, const char *formact)
{
    return QImage::loadFromData(data, formact);
}

inline bool WImage::isNull() const
{
    return QImage::isNull();
}

inline RectF WImage::rect() const
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

inline bool WImage::operator==(const WImage &other) const
{
    return QImage::operator==(other);
}

inline int WImage::width() const
{
    return QImage::width();
}

inline int WImage::height() const
{
    return QImage::height();
}

inline WImage &WImage::operator=(const QImage &other)
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

inline WRgb WImage::pixel(const WPoint &point) const
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

inline WImage &WImage::operator=(WImage &&other) noexcept
{
    if (this == &other)
        return *this;
    QImage::operator=(std::move(other));
    return *this;
}

inline WImage::WImage(const WImage &other) noexcept = default;

#endif // USE_QT

