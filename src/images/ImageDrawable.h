#pragma once

#include "core/WImage.h"

class ImageDrawable {
public:
    // TODO: make this variable private
    WImage immagini;
    PointF i;
    PointF f;

    static auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> std::pair<int, ImageDrawable>;
    static auto write (WritableAbstract &writable, const ImageDrawable &source) noexcept -> int;

};

