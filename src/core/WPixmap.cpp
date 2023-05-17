#include "WPixmap.h"

#include "touch/dataTouch/page/Page.h"

#if defined(USE_QT)
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

WPixmap::WPixmap(const WString &path, const char *format)
    : QPixmap(path, format)
{
}

size_t WPixmap::get_size_in_file() const
{
    WByteArray arr;
    const auto s = this->save_and_size(arr);
    return s;
}

size_t WPixmap::save_and_size(WByteArray &arr) const
{
    W_ASSERT(arr.size() == 0);
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    QPixmap::save(&buffer, "PNG");
    buffer.close();
    return arr.size();
}
#endif // USE_QT