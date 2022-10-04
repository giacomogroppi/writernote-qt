#include "WImage.h"
#include "utils/common_script.h"
#include "touch/dataTouch/page/Page.h"
#include <QList>

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
    const auto s = this->save_and_size(arr);
    return s;
}

WImage::WImage(int width, int height, QImage::Format format) : QImage(width, height, format)
{
}

WImage::WImage(int page) :
    QImage(Page::getResolutionWidth(), Page::getResolutionHeigth() * page, WImage::Format_ARGB32)
{
    W_ASSERT(page > 0);
    const auto res = QImage::isNull();
    W_ASSERT(!res);
}
