#pragma once

#include <functional>
#include <memory>

#include "VersionFileController.h"
#include "utils/WCommonScript.h"
#include "Writable.h"
#include "Readable.h"

template <class T>
class SharedPtr: public std::shared_ptr<T> {
public:
    template <class Readable> requires (std::is_base_of_v<ReadableAbstract, Readable>)
    static auto load (const VersionFileController &version, Readable &readable, SharedPtr<T> &result) -> int
    {
        bool is_value_present;

        if (version.getVersionSharedPtr() != 0)
            return -1;

        if (readable.read(is_value_present) < 0) {

        }
    }

    /**
     * @return &lt 0 in case Writable fail
     * */
    template <class Writable> requires(std::is_base_of_v<WritableAbstract, Writable>)
    static auto save (Writable &writable, const SharedPtr<T> &object) -> int
    {
        const bool is_present = object.get() != nullptr;
        const T& ref = *object.get();

        if (writable.write(&is_present, sizeof (is_present)) < 0)
            return -1;

        if (is_present and T::save (writable, ref) < 0) {
            return -1;
        }

        return 0;
    }
};
