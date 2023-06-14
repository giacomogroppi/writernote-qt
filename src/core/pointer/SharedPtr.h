#pragma once

#include <functional>
#include <memory>

#include "VersionFileController.h"
#include "utils/WCommonScript.h"

template <class T>
class SharedPtr: public std::shared_ptr<T> {
public:
    template <class Readable>
    static int load (const VersionFileController &version, Readable &readable, SharedPtr<T> &result)
    {
        W_ASSERT(0);
        return -1;
    }
};