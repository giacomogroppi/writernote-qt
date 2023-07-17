#pragma once

#include <iostream>
#include <type_traits>

#include "Writable.h"
#include "Readable.h"

class VersionFileController {
private:
#   define DEFINE_VERSION(name, version)                                        \
    private:                                                                    \
    static constexpr unsigned short currentVersion##name = version;             \
    unsigned short _version##name;                                              \
    public:                                                                     \
    constexpr auto getVersion##name() const -> int { return _version##name; }

    DEFINE_VERSION(WListFast, 0);
    DEFINE_VERSION(WString, 0);
    DEFINE_VERSION(WPair, 0);
    DEFINE_VERSION(SharedPtr, 0);
    DEFINE_VERSION(WByteArray, 0);
    DEFINE_VERSION(DataStruct, 0);
    DEFINE_VERSION(PointTemplate, 0);
    DEFINE_VERSION(Page, 0);
    DEFINE_VERSION(Stroke, 0);
    DEFINE_VERSION(StrokeCircle, 0);
    DEFINE_VERSION(RectTemplate, 0);
    DEFINE_VERSION(MetadataStroke, 0);
    DEFINE_VERSION(StrokeNormal, 0);
    DEFINE_VERSION(StrokeLine, 0);
    DEFINE_VERSION(StrokeRect, 0 );
    DEFINE_VERSION(Pressure, 0);
    DEFINE_VERSION(WColor, 0);
    DEFINE_VERSION(StrokeForPage, 0);
    DEFINE_VERSION(WPixmap, 0);
    DEFINE_VERSION(ImageDrawable, 0);
    DEFINE_VERSION(WImage, 0);

public:
    VersionFileController() = default;

    [[deprecated("This copy constructor will be removed")]]
    VersionFileController (const VersionFileController &other) noexcept = default;

    VersionFileController (VersionFileController &&other) noexcept = default;

    static auto load (ReadableAbstract &readable) -> std::pair<int, VersionFileController>;

    auto operator=(VersionFileController &&other) noexcept -> VersionFileController& = default;
};

inline auto VersionFileController::load(ReadableAbstract &readable) -> std::pair<int, VersionFileController>
{
    VersionFileController result{};
    short versionVersionFileController;

    if (readable.read(versionVersionFileController) < 0)
        return {-1, result};

    unsigned short *d[] = {
    };

    for (auto & i : d) {
        if (readable.read (&i, sizeof (*d)) < 0)
            return {-1, result};
    }

    return {0, result};
}
