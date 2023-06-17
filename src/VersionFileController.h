#pragma once

#include <iostream>
#include <type_traits>

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
            currentversionSharedPtr = 0;


    unsigned short _versionWListFast;
    unsigned short _versionWString;
    unsigned short _versionWPair;
    unsigned short _versionSharedPtr;
public:
    /**
     * It's required for template Readable to have read method
     * */
    template <class Readable>
    static VersionFileController loadVersion (Readable &readable);


    constexpr auto getVersionWListFast()     const noexcept -> int { return this->_versionWListFast; };
    constexpr auto getVersionWString()       const noexcept -> int { return this->_versionWString; };
    constexpr auto getVersionWPair()         const noexcept -> int { return this->_versionWPair; };
    constexpr auto getVersionSharedPtr()     const noexcept -> int { return this->_versionSharedPtr; }
};
