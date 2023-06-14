#pragma once

#include <iostream>
#include "core/String/WString.h"

class WTime {
    int _hour, _minute, _second;
public:
    WTime (int h, int m, int s);

    static WTime now ();

    [[nodiscard]]
    WString toString() const;
};

class WDate {
    char _day;
    char _mount;
    short _year;
public:
    WDate();
    WDate(int year, int mount, int day);

    static WDate now();

    [[nodiscard]]
    WString toString() const;
};
