#pragma once

#ifdef USE_QT
#include <QImage>
#include <QBuffer>
#include "core/WByteArray.h"
#else
#include "core/WByteArray.h"
#endif

#include "core/WByteArray.h"
#include "RectF.h"
#include "WString.h"
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
    WImage(const WString &path);
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

#ifdef USE_QT
    [[nodiscard]] QImage toQImage() const;

    WImage &operator=(const QImage &other);
#endif // USE_QT

    friend class WPainter;

    WRgb pixel(const Point &point) const;
};

#ifdef USE_QT

inline bool WImage::operator==(const WImage &other) const
{
    return QImage::operator==(other);
}
#endif

#ifdef USE_QT
inline WImage &WImage::operator=(const QImage &other)
{
    if (this == &other)
        return *this;
    QImage::operator==(other);
    return *this;
}
#endif

