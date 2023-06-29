#include "WImage.h"
#include "utils/WCommonScript.h"
#include "touch/dataTouch/page/Page.h"

#ifdef USE_QT
#include <QImage>
#include <QBuffer>
#include <QString>

WImage::WImage(const std::string &path, const char *format):
    QImage(path.c_str(), format)
{
    this->fill(Qt::transparent);
}
#else
#endif

#ifdef USE_QT
size_t WImage::save_and_size(WByteArray &arr) const
{
    W_ASSERT(arr.size() == 0);
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    QImage::save(&buffer, "PNG");
    buffer.close();
    return arr.size();
}
#endif

size_t WImage::get_size_in_file() const
{
    WByteArray arr;
    const auto s = this->save_and_size(arr);
    return s;
}

#ifdef USE_QT

WImage::WImage(int width, int height, WImageType format)
    : QImage(
        width,
        height,
        format == WImageType::png ? QImage::Format_ARGB32 : QImage::Format_ARGB32)
{
    W_ASSERT(format == WImageType::png);
}

WImage::WImage(int page, bool consideringResolution) :
    QImage(consideringResolution
               ? Page::getResolutionWidth()
               : Page::getWidth(),
           static_cast<int>(consideringResolution
                                ? Page::getResolutionHeigth()
                                : Page::getHeight()
                            ) * page,
           WImage::Format_ARGB32)
{
    W_ASSERT(page > 0);
    const auto res = QImage::isNull();
    W_ASSERT(!res);
}

auto
WImage::load(const VersionFileController &versionController, ReadableAbstract &readable) -> std::pair<int, WImage>
{
    WImage result;

    if (versionController.getVersionWImage() != 0)
        return {-1, {}};

    auto [res, raw] = WByteArray::load(versionController, readable);
    if (res < 0)
        return {-1, {}};

    if (!result.loadFromData(raw, "PNG"))
        return {-1, {}};

    return {0, std::move(result)};
}
#else

#endif // USE_QT

auto WImage::write(WritableAbstract &writable, const WImage &image) -> int
{
    if (WByteArray::write(writable, image.getRawDataPNG()) < 0)
        return -1;
    return 0;
}