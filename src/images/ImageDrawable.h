#pragma once

#include "core/Image/WImage.h"
#include "core/PointF.h"

class ImageDrawable {
public:
    // TODO: make this variable private
    WImage immagini;
    PointF i;
    PointF f;

    static auto load (const VersionFileController &versionController,
                      ReadableAbstract &readable) -> WPair<Error, ImageDrawable>;
    static auto write (WritableAbstract &writable, const ImageDrawable &source) noexcept -> Error;

};

