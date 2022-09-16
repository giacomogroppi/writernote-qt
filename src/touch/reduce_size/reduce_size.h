#pragma once


class datastruct;
#include "stdio.h"

class reduce_size
{
public:
    static size_t current_size(datastruct *);
    static size_t calculate_min_size(datastruct *);

    static void decrese(datastruct *);

};

