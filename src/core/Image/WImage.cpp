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

size_t WImage::save_and_size(WByteArray &arr) const
{
    W_ASSERT(arr.size() == 0);
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    QImage::save(&buffer, "PNG");
    buffer.close();
    return arr.size();
}

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
#endif // USE_QT

auto WImage::load(const VersionFileController &versionController,
                  ReadableAbstract &readable) -> WPair<Error, WImage>
{
    WImage result;

    if (versionController.getVersionWImage() != 0)
        return {Error::makeErrVersion(), {}};

    auto [res, raw] = WByteArray::load(versionController, readable);
    if (res)
        return {res, {}};

    if (!result.loadFromData(raw, "PNG"))
        return {Error::makeCorruption(), {}};

    return {Error::makeOk(), std::move(result)};
}

auto WImage::write(WritableAbstract &writable, const WImage &image) -> Error
{
    if (auto err = WByteArray::write(writable, image.getRawDataPNG()))
        return err;
    return Error::makeOk();
}
