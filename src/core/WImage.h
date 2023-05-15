#pragma once

#ifdef USE_QT
#include <QImage>
#include <QBuffer>
#include "core/WByteArray.h"
#else
#include "core/WByteArray.h"
#endif

#include "core/WByteArray.h"
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
    explicit WImage(const std::string &fileName, const char *format = nullptr);
    explicit WImage(int page, bool consideringResolution);
    WImage(int width, int height, WImageType format);
    WImage() = default;

    [[nodiscard]] size_t get_size_in_file() const;
    [[nodiscard]] size_t save_and_size(WByteArray &arr) const;

#ifdef USE_QT
    [[nodiscard]] QImage toQImage() const;
#endif

    bool operator==(const WImage &other) const;

#ifdef USE_QT
    WImage &operator=(const QImage &other);
#endif // USE_QT

    friend class WPainter;
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

