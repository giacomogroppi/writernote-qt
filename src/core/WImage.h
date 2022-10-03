#pragma once

#include <QImage>
#include <QBuffer>
#include <QByteArray>

class WImage: public QImage {
public:
    explicit WImage(const QString &fileName, const char *format = nullptr);
    explicit WImage(int page);
    WImage(int width, int height, QImage::Format format);
    WImage() = default;

    [[nodiscard]] size_t get_size_in_file() const;
    [[nodiscard]] size_t save_and_size(QByteArray &arr) const;
};
