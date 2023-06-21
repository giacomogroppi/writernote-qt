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
    static constexpr unsigned short
            currentVersionWListFast = 0,
            currentVersionWString = 0,
            currentVersionWPair = 0,
            currentVersionSharedPtr = 0,
            currentVersionWByteArray = 0,
            currentVersionDataStruct = 0,
            currentVersionPointTemplate = 0,
            currentVersionPage = 0,
            currentVersionStroke = 0,
            currentVersionStrokeCirle = 0;

    long _isOk;
    unsigned short _versionWListFast;
    unsigned short _versionWString;
    unsigned short _versionWPair;
    unsigned short _versionSharedPtr;
    unsigned short _versionWByteArray;
    unsigned short _versionDataStruct;
    unsigned short _versionPointTemplate;
    unsigned short _versionPage;
    unsigned short _versionStroke;
    unsigned short _versionStrokeCircle;

public:
    VersionFileController() = default;

    VersionFileController (VersionFileController &&other) noexcept;

    /**
     * It's required for template Readable to have read method
     * */
    template <class Readable>
            requires (std::is_base_of_v<ReadableAbstract, Readable>)
    static VersionFileController loadVersion (Readable &readable);

    /**
     * \return true iff it's load corrently
     * */
    bool isOk() const noexcept;

    constexpr auto getVersionWListFast()     const noexcept -> int { return this->_versionWListFast; };
    constexpr auto getVersionWString()       const noexcept -> int { return this->_versionWString; };
    constexpr auto getVersionWPair()         const noexcept -> int { return this->_versionWPair; };
    constexpr auto getVersionSharedPtr()     const noexcept -> int { return this->_versionSharedPtr; }
    constexpr auto getVersionWByteArray()    const noexcept -> int { return _versionWByteArray; }
    constexpr auto getVersionDataStruct()    const noexcept -> int { return _versionDataStruct; };
    constexpr auto getVersionPointTemplate() const noexcept -> int { return _versionPointTemplate; };
    constexpr auto getVersionPage()          const noexcept -> int { return _versionPage; }
    constexpr auto getVersionStroke()        const noexcept -> int { return _versionStroke; }
    constexpr auto getVersionStrokeCircle()  const noexcept -> int { return _versionStrokeCircle; };
    constexpr auto getVersionRectTemplate()  const noexcept -> int;
    constexpr auto getVersionMetadataStroke() const noexcept -> int;
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
            &result._versionWListFast,
            &result._versionWString,
            &result._versionWPair,
            &result._versionSharedPtr,
            &result._versionWByteArray,
            &result._versionDataStruct,
            &result._versionPointTemplate,
            &result._versionPage,
            &result._versionStroke,
            &result._versionStrokeCircle
    };

    for (int i = 0; i < sizeof (d); i++) {
        if (readable.read (&d[i], sizeof (*d)) < 0)
            result._isOk = 0;
    }

    return result;
}
