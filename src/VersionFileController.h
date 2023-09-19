#pragma once

#include <iostream>
#include <type_traits>

#include "Writable.h"
#include "Readable.h"
#include "utils/WCommonScript.h"

class VersionFileController {
private:
#   define DEFINE_VERSION(name, version)                                        \
    private:                                                                    \
    static constexpr unsigned short currentVersion##name = version;             \
    unsigned short _version##name;                                              \
    public:                                                                     \
    constexpr auto getVersion##name() const -> int { return _version##name; }   \
    private:

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
    DEFINE_VERSION(WMap, 0);

    static constexpr auto numberOfElements = 22;

    static
    auto getArray(VersionFileController &versionController) -> std::unique_ptr<unsigned short *>;
public:
    VersionFileController() = default;

    VersionFileController (const VersionFileController &other) noexcept = default;
    VersionFileController (VersionFileController &&other) noexcept = default;

    static auto load (ReadableAbstract &readable) -> std::pair<int, VersionFileController>;
    static auto write (WritableAbstract &writable) -> int;
    
    auto operator=(VersionFileController &&other) noexcept -> VersionFileController& = default;
    auto operator=(const VersionFileController &other) noexcept -> VersionFileController& = default;
};

inline auto VersionFileController::getArray(VersionFileController &versionController) -> std::unique_ptr<unsigned short *>
{
    auto array = new unsigned short* [numberOfElements]{
        &versionController._versionWListFast,
        &versionController._versionWString,
        &versionController._versionWPair,
        &versionController._versionSharedPtr,
        &versionController._versionWByteArray,
        &versionController._versionDataStruct,
        &versionController._versionPointTemplate,
        &versionController._versionPage,
        &versionController._versionStroke,
        &versionController._versionStrokeCircle,
        &versionController._versionRectTemplate,
        &versionController._versionMetadataStroke,
        &versionController._versionStrokeNormal,
        &versionController._versionStrokeLine,
        &versionController._versionStrokeRect,
        &versionController._versionPressure,
        &versionController._versionWColor,
        &versionController._versionStrokeForPage,
        &versionController._versionWPixmap,
        &versionController._versionImageDrawable,
        &versionController._versionWImage,
        &versionController._versionWMap
    };
    static_assert(sizeof (VersionFileController) == sizeof(unsigned short) * numberOfElements);

    return std::unique_ptr<unsigned short*>(array);
}

inline auto VersionFileController::load(ReadableAbstract &readable) -> std::pair<int, VersionFileController>
{
    VersionFileController result{};
    short versionVersionFileController;

    if (readable.read(versionVersionFileController) < 0)
        return {-1, result};

    std::unique_ptr<unsigned short*> d = getArray(result);

    for (int i = 0; i < numberOfElements; i++) {
        if (readable.read (&(*d)[i], sizeof (unsigned short)) < 0)
            return {-1, result};
    }

    return {0, result};
}

inline auto VersionFileController::write(WritableAbstract& writable) -> int
{
    VersionFileController object{};
    auto array = getArray(object);

    for (int i = 0; i < VersionFileController::numberOfElements; ++i) {
        if (writable.write(&(*array)[i], sizeof(unsigned short)) < 0)
            return -1;
    }

    return 0;
}
