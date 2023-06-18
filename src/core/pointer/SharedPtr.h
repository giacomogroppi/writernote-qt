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
    SharedPtr() noexcept = default;
    SharedPtr(SharedPtr<T> &&other) noexcept;
    SharedPtr(const SharedPtr<T> &other) noexcept;
    SharedPtr(std::shared_ptr<T> &&object) noexcept;
    SharedPtr(const std::shared_ptr<T> &object) noexcept;
    SharedPtr(T *object) noexcept;

    template<class Z>
        requires (std::is_const<T>())
    SharedPtr(SharedPtr<Z> &other);


    template <class Readable> requires (std::is_base_of_v<ReadableAbstract, Readable>)
    static auto load (const VersionFileController &version, Readable &readable, SharedPtr<T> &result) -> int
    {
        bool is_value_present;

        if (version.getVersionSharedPtr() != 0)
            return -1;

        if (readable.read(is_value_present) < 0) {
            return -1;
        }

        result = SharedPtr<T>(new T());
        //SharedPtr<T> tmp = std::shared_ptr<T>(new T());

        if (T::load(version, readable, *result.get()) < 0) {
            return -1;
        }

        //result = std::move(tmp);

        return 0;
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

    auto operator=(const SharedPtr<T> &other) noexcept -> SharedPtr<T> &;
    auto operator=(SharedPtr<T> &&other) noexcept -> SharedPtr<T> &;

    auto operator=(const SharedPtr<T> &other) const noexcept -> const SharedPtr<T> &;
    auto operator=(SharedPtr<T> &&other) const noexcept -> const SharedPtr<T> &;

    // T const
    template <class Z> requires (std::is_const<T>())
    auto operator=(const SharedPtr<Z> &other) noexcept -> SharedPtr<T> &;
};

template<class T>
template<class Z>
    requires(std::is_const<T>())
inline SharedPtr<T>::SharedPtr(SharedPtr<Z> &other)
     : std::shared_ptr<T>(other)
{

}

template<class T>
inline auto SharedPtr<T>::operator=(const SharedPtr<T> &other) const noexcept -> const SharedPtr<T> &
{
    if (this == &other) return *this;
    std::shared_ptr<T>::operator=(other);
    return *this;
}

template<class T>
inline auto SharedPtr<T>::operator=(SharedPtr<T> &&other) const noexcept -> const SharedPtr<T> &
{
    if (this == &other) return *this;
    std::shared_ptr<T>::operator=(std::move(other));
    return *this;
}

template<class T>
inline SharedPtr<T>::SharedPtr(T *object) noexcept
    : std::shared_ptr<T>(object)
{

}

template <class T>
inline auto SharedPtr<T>::operator=(SharedPtr<T> &&other) noexcept -> SharedPtr<T> &
{
    if (this == &other) return *this;
    std::shared_ptr<T>::operator=(std::move(other));
    return *this;
}

template <class T>
inline auto SharedPtr<T>::operator=(const SharedPtr<T> &other) noexcept -> SharedPtr<T> &
{
    if (this == &other) return *this;
    std::shared_ptr<T>::operator=(other);
    return *this;
}

template<class T>
inline SharedPtr<T>::SharedPtr(const std::shared_ptr<T> &object) noexcept
    : std::shared_ptr<T>(object)
{

}

template<class T>
inline SharedPtr<T>::SharedPtr(std::shared_ptr<T> &&object) noexcept:
    std::shared_ptr<T>(std::move(object))
{

}

