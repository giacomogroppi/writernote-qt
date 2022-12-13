#pragma once

#include <QObject>
#include <QtGlobal>

#include "utils/WCommonScript.h"

namespace W{

template <class T>
class unique_ptr
{
private:
    T* _pointer;
public:
    unique_ptr(T *p) {
        W_ASSERT(p);
        _pointer = p;
    };
};

}
