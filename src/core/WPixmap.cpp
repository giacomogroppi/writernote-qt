#include "WPixmap.h"

#include "touch/dataTouch/page/Page.h"

#ifdef USE_QT
WPixmap::WPixmap(int page, bool consideringResolution)
    : QPixmap(
        consideringResolution
        ? Page::getResolutionWidth()
        : Page::getWidth(),
        static_cast<int>(consideringResolution
                         ? Page::getResolutionHeigth()
                         : Page::getHeight()
        ) * page)
{

}

WPixmap::WPixmap(const std::string &path, const char *format)
    : QPixmap(QString(path.c_str()), format)
{

}

void WPixmap::fill(const WColor &color)
{
    QPixmap::fill(color.toQColor());
}

WImage WPixmap::toImage() const
{
    return {QPixmap::toImage()};
}

bool WPixmap::operator==(const WPixmap &other) const
{
    return QPixmap::toImage() == other.toImage();
}

bool WPixmap::loadFromData(const WByteArray &data, const char *formact)
{
    return QPixmap::loadFromData(data, formact);
}

bool WPixmap::isNull() const
{
    return QPixmap::isNull();
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

Rect WPixmap::rect() const
{
    const auto qtRect = QPixmap::rect();

    const auto qtTopLeft = qtRect.topLeft();
    const auto qtBottomRight = qtRect.bottomRight();

    return Rect {
        qtTopLeft.x(),
        qtTopLeft.x(),
        qtBottomRight.x(),
        qtBottomRight.y()
    };
}

WPixmap::WPixmap(const WString &path)
    : QPixmap()
{
    WImage img(path);
    WPixmap::fromImage(img);
}

#endif
