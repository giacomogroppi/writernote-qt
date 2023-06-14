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

public:
    /**
     * It's required for template Readable to have read method
     * */
    template <class Readable>
    static VersionFileController loadVersion (Readable &readable);


    int versionWListFast() const;

    constexpr int versionWString() const noexcept;
};
