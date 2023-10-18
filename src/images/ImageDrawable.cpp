#include "ImageDrawable.h"

auto ImageDrawable::load(const VersionFileController &versionController,
                         ReadableAbstract &readable) -> WPair<Error, ImageDrawable>
{
    ImageDrawable result;

    if (versionController.getVersionImageDrawable() != 0)
        return {Error::makeErrVersion(), {}};

    {
        auto [res, image] = WImage::load(versionController, readable);
        if (res)
            return {res, {}};
        result.immagini = std::move(image);
    }

    {
        auto [res, point] = PointF::load(versionController, readable);
        if (res)
            return {res, {}};
        result.i = std::move(point);
    }

    {
        auto [res, point] = PointF::load(versionController, readable);
        if (res)
            return {res, {}};
        result.f = std::move(point);
    }

    return {Error::makeOk(), result};
}

auto ImageDrawable::write(WritableAbstract &writable, 
                            const ImageDrawable &source) noexcept -> Error
{
    if (auto err = WImage::write(writable, source.immagini))
        return err;
    if (auto err = PointF::write(writable, source.i))
        return err;
    if (auto err = PointF::write(writable, source.f))
        return err;
    return Error::makeOk();
}
