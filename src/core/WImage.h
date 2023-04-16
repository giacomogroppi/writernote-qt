#pragma once

#include <QImage>
#include <QBuffer>
#include <QByteArray>

class WImage: public QImage {
private:
#ifdef Q_OS_IOS
    /**
     * this is a NSImage object
    */
    void *d;
#endif
public:
    explicit WImage(const QString &fileName, const char *format = nullptr);
    explicit WImage(int page, bool consideringResolution);
    WImage(int width, int height, QImage::Format format);
    WImage() = default;

    [[nodiscard]] size_t get_size_in_file() const;
    [[nodiscard]] size_t save_and_size(QByteArray &arr) const;

    [[nodiscard]] QImage toQImage() const;

    bool operator==(const WImage &other) const;
    WImage &operator=(const QImage &other);

    friend class WPainter;
};

inline bool WImage::operator==(const WImage &other) const
{
    return QImage::operator==(other);
}

inline WImage &WImage::operator=(const QImage &other)
{
    if (this == &other)
        return *this;

    QImage::operator==(other);
    return *this;
}

