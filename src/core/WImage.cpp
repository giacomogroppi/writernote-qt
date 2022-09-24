#include "WImage.h"
#include "utils/common_script.h"

WImage::WImage(const QString &path, const char *format):
    QImage(path, format)
{
}

size_t WImage::save_and_size(QByteArray &arr) const
{
    W_ASSERT(arr.size() == 0);
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    QImage::save(&buffer, "PNG");
    buffer.close();
    return arr.size();
}

size_t WImage::get_size_in_file() const
{
    QByteArray arr;
    return this->save_and_size(arr);
}
