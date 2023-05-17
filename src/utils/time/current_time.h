#pragma once

#include <iostream>
#include "core/WString.h"

class WTime {
public:
    WTime (int h, int m, int s);

    static WTime now ();

    WString toString();
};

class WDate {
public:
    WDate();

    static WDate now();

    WString toString();
};
