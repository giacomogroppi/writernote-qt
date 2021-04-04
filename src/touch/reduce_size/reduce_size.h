#ifndef REDUCE_SIZE_H
#define REDUCE_SIZE_H

class datastruct;
#include "stdio.h"

class reduce_size
{
public:
    static size_t current_size(datastruct *);
    static size_t calculate_min_size(datastruct *);

    static void decrese(datastruct *);

};

#endif // REDUCE_SIZE_H
