#pragma once

#include <iostream>
#include "core/String/WString.h"

class WTime {
    int _hour, _minute, _second;
public:
    WTime (int h, int m, int s);
    WTime (const WTime &other) = default;
    WTime (WTime &&other) = default;

    static auto now() -> WTime;

    [[nodiscard]]
    auto toString() const -> WString;
};

class WDate {
    char _day;
    char _mount;
    short _year;
public:
    WDate();
    WDate(int year, int mount, int day);

    WDate(const WDate &other) = default;
    WDate(WDate &&other) = default;

    static auto now() -> WDate;

    [[nodiscard]]
    auto toString() const -> WString;

    auto operator=(WDate &&other) noexcept -> WDate & = default;
    auto operator=(const WDate &other) noexcept -> WDate & = default;
};
