#pragma once

#include "utils/common_def.h"
#include "stdlib.h"

template <typename T>
class multiarray
{
private:
    T **data;
    int lenLine;
    int *lenPerLine;
public:
    multiarray(const T &init);
    multiarray(const multiarray<T> &list);
    ~multiarray();
    void addLine(const T *line, cint numElement);
};

template<typename T>
inline void multiarray<T>::addLine(const T *line, const int numElement)
{
    lenLine ++;
    lenPerLine = (int *)realloc(lenPerLine, sizeof(*lenPerLine) * lenLine);
    lenPerLine[lenLine - 1] = numElement;

    data = (T **)realloc(data, sizeof(T *) * lenLine);
    data[lenLine - 1] = (T *)realloc(data[lenLine - 1], sizeof(T) * numElement);
    memcpy(data[lenLine - 1], line, sizeof(T) * numElement);
}


