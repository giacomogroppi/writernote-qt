#pragma once

#include <iostream>

class WTime {
public:
    WTime (int h, int m, int s);

    static WTime now ();

    std::string toString();
};

class WDate {
public:
    WDate(int y, int m, int d);

    static WDate now();

    std::string toString();
};
