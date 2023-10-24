#include "WPixmap.h"

#include "touch/dataTouch/page/Page.h"

#ifdef USE_QT
WPixmap::WPixmap(WPixmap &&other) noexcept
    : QPixmap(std::move(static_cast<QPixmap &&>(other)))
{

}

WPixmap::WPixmap(const WPixmap &other) noexcept
    : QPixmap(static_cast<const QPixmap &>(other))
{

}

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

auto WPixmap::toImage() const -> WImage
{
    return WImage {
        QPixmap::toImage()
    };
}

auto WPixmap::operator==(const WPixmap &other) const -> bool
{
    const QImage image1 = toImage();
    const QImage image2 = other.toImage();
    return image1 == image2;
}

auto WPixmap::loadFromData(const WByteArray &data, const char *formact) -> bool
{
    return QPixmap::loadFromData(data, formact);
}

auto WPixmap::isNull() const -> bool
{
    return QPixmap::isNull();
}

auto WPixmap::getSizeInFile(const WPixmap &source) -> size_t
{
    WByteArray arr;
    const auto s = source.save_and_size(arr);

    return s;
}

auto WPixmap::save_and_size(WByteArray &arr) const -> size_t
{
    W_ASSERT(arr.size() == 0);
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    QPixmap::save(&buffer, "PNG");
    buffer.close();
    return arr.size();
}

auto WPixmap::rect() const -> WRect
{
    const auto qtRect = QPixmap::rect();

    const auto qtTopLeft = qtRect.topLeft();
    const auto qtBottomRight = qtRect.bottomRight();

    WDebug(false, qtTopLeft.x() << qtTopLeft.y() << qtBottomRight.x() << qtBottomRight.y());

    auto res = WRect {
        qtTopLeft.x(),
        qtTopLeft.y(),
        qtBottomRight.x(),
        qtBottomRight.y()
    };

    W_ASSERT(res.bottomRight().x() == qtBottomRight.x());
    return res;
}

WPixmap::WPixmap(const WString &path)
    : QPixmap()
{
    WImage img(path);
    WPixmap::fromImage(img);
}

WPixmap &WPixmap::operator=(const WPixmap &other) noexcept
{
    if (this == &other)
        return *this;
    QPixmap::operator=(other);
    return *this;
}

WPixmap &WPixmap::operator=(WPixmap &&other) noexcept
{
    if (this == &other)
        return *this;
    QPixmap::operator=(std::move(other));
    return *this;
}

auto WPixmap::load(const VersionFileController &versionController,
                   ReadableAbstract &readable) -> WPair<Error, WPixmap>
{
    if (versionController.getVersionWPixmap() != 0)
        return {Error::makeErrVersion(), {}};

    WPixmap result;

    auto [res, data] = WByteArray::load(versionController, readable);

    if (res)
        return {res, {}};

    if (data.isEmpty())
        return {Error::makeOk(), WPixmap()};

    if (not result.loadFromData(data, "PNG"))
        return {Error::makeCorruption(), {}};

    return {Error::makeOk(), result};
}

auto WPixmap::getRawDataPNG() const -> WByteArray
{
    QByteArray arr;
    QBuffer buffer(&arr);

    buffer.open(QIODevice::WriteOnly);
    QPixmap::save(&buffer, "PNG");
    buffer.close();

    return arr;
}

auto WPixmap::write(WritableAbstract &writable, const WPixmap &pixmap) -> Error
{
    const auto raw = pixmap.getRawDataPNG();

    WDebug(true, raw.size());

    if (auto err = WByteArray::write(writable, raw))
        return err;

    return Error::makeOk();
}

#endif // USE_QT
