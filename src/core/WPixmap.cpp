#include "WPixmap.h"

#include "touch/dataTouch/page/Page.h"
#include <QPixmap>

WPixmap::WPixmap(int page, bool consideringResolution)
    : QPixmap(consideringResolution
                 ? Page::getResolutionWidth()
                 : Page::getWidth(),
             static_cast<int>(consideringResolution
                                  ? Page::getResolutionHeigth()
                                  : Page::getHeight()
                              ) * page)
{

}

WPixmap::WPixmap(const QString &path, const char *format)
    : QPixmap(path, format)
{
}

size_t WPixmap::get_size_in_file() const
{
    QByteArray arr;
    const auto s = this->save_and_size(arr);
    return s;
}

size_t WPixmap::save_and_size(QByteArray &arr) const
{
    W_ASSERT(arr.size() == 0);
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    QPixmap::save(&buffer, "PNG");
    buffer.close();
    return arr.size();
}
