#pragma once

#include <iostream>
#include <type_traits>

#include "Writable.h"
#include "Readable.h"
#include "utils/WCommonScript.h"

class VersionFileController
{
private:
#   define DEFINE_VERSION(name, version)                                        \
    private:                                                                    \
    static constexpr unsigned short currentVersion##name = version;             \
    unsigned short _version##name = version;                                    \
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
    DEFINE_VERSION(WAbstractList, 0);
    DEFINE_VERSION(WElement, 0);
    DEFINE_VERSION(AudioPosition, 0);

    static constexpr auto numberOfElements = 25;
    static constexpr unsigned short versionVersionFileController = 0u;

    static
    auto getArray(VersionFileController &versionController) -> std::unique_ptr<unsigned short *>;
public:
    VersionFileController() = default;

    VersionFileController (const VersionFileController &other) noexcept = default;
    VersionFileController (VersionFileController &&other) noexcept = default;

    [[nodiscard]]
    static auto load (ReadableAbstract &readable) -> std::pair<Error, VersionFileController>;

    nd
    static auto write (WritableAbstract &writable) -> Error;
    
    auto operator=(VersionFileController &&other) noexcept -> VersionFileController& = default;
    auto operator=(const VersionFileController &other) noexcept -> VersionFileController& = default;

    auto operator==(const VersionFileController& other) const noexcept -> bool = default;
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
        &versionController._versionWMap,
        &versionController._versionWAbstractList,
        &versionController._versionWElement,
        &versionController._versionAudioPosition
    };
    static_assert(sizeof (VersionFileController) == sizeof(unsigned short) * numberOfElements);

    return std::unique_ptr<unsigned short*>(array);
}

inline auto VersionFileController::load(ReadableAbstract &readable) -> std::pair<Error, VersionFileController>
{
    VersionFileController result{};
    unsigned short version;

    if (auto err = readable.read(version))
        return {err, result};

    std::unique_ptr<unsigned short*> d = getArray(result);

    for (int i = 0; i < numberOfElements; i++) {
        unsigned short &value = (*d)[i];

        if (auto err = readable.read (value))
            return {err, result};
    }

    return {Error::makeOk(), std::move(result)};
}

inline auto VersionFileController::write(WritableAbstract& writable) -> Error
{
    VersionFileController object{};
    auto array = getArray(object);

    if (auto err = writable.write(versionVersionFileController))
        return err;

    for (int i = 0; i < VersionFileController::numberOfElements; ++i) {
        unsigned short value = (*array)[i];

        if (auto err = writable.write(value))
            return err;
    }

    return Error::makeOk();
}
