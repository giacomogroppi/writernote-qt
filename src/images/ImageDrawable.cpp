#include "ImageDrawable.h"

auto ImageDrawable::load(const VersionFileController &versionController,
                         ReadableAbstract &readable) -> WPair<int, ImageDrawable>
{
    ImageDrawable result;

    if (versionController.getVersionImageDrawable() != 0)
        return {-1, {}};

    {
        auto [res, image] = WImage::load(versionController, readable);
        if (res < 0)
            return {-1, {}};
        result.immagini = std::move(image);
    }

    {
        auto [res, point] = PointF::load(versionController, readable);
        if (res < 0)
            return {-1, {}};
        result.i = std::move(point);
    }

    {
        auto [res, point] = PointF::load(versionController, readable);
        if (res < 0)
            return {-1, {}};
        result.f = std::move(point);
    }

    return {0, result};
}

auto ImageDrawable::write(WritableAbstract &writable, const ImageDrawable &source) noexcept -> int
{
    if (WImage::write(writable, source.immagini) < 0)
        return -1;
    if (PointF::write(writable, source.i) < 0)
        return -1;
    if (PointF::write(writable, source.f) < 0)
        return -1;
    return 0;
}
