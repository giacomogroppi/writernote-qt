#pragma once

#include <iostream>
#include <type_traits>

#include "Writable.h"
#include "Readable.h"

// SFINAE test
template <typename T>
class HasReadFunction
{
private:
    typedef char YesType[1];
    typedef char NoType[2];

    template <typename C> static YesType& test( decltype(&C::read) ) ;
    template <typename C> static NoType& test(...);
public:
    enum { value = sizeof(test<T>(0)) == sizeof(YesType) };
};

class VersionFileController {
private:
#   define DEFINE_VERSION(name, version)                                        \
    private:                                                                    \
    static constexpr unsigned short currentVersion##name = version;             \
    unsigned short _version##name;                                              \
    public:                                                                     \
    constexpr auto getVersion##name() const -> int { return _version##name; }

    long _isOk;
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

public:
    VersionFileController() = default;

    VersionFileController (VersionFileController &&other) noexcept = default;

    /**
     * It's required for template Readable to have ReadableAbstract as base class
     * */
    template <class Readable>
            requires (std::is_base_of_v<ReadableAbstract, Readable>)
    static VersionFileController loadVersion (Readable &readable);

    /**
     * \return true iff it's load corrently
     * */
    bool isOk() const noexcept;
};

template<class Readable>
    requires (std::is_base_of_v<ReadableAbstract, Readable>)
inline VersionFileController VersionFileController::loadVersion(Readable &readable)
{
    VersionFileController result{};
    short versionVersionFileController;

    result._isOk = readable.read(&versionVersionFileController, sizeof (versionVersionFileController)) >= 0;

    if (!result._isOk)
        return result;

    unsigned short *d[] = {
    };

    for (int i = 0; i < sizeof (d); i++) {
        if (readable.read (&d[i], sizeof (*d)) < 0)
            result._isOk = 0;
    }

    return result;
}
