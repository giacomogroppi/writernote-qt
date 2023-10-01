#pragma once

#include <functional>
#include <memory>

#include "VersionFileController.h"
#include "utils/WCommonScript.h"
#include "Writable.h"
#include "Readable.h"
#include "core/WPair.h"

template <class T>
class SharedPtr: public std::shared_ptr<T>
{
public:
    SharedPtr() noexcept = default;
    SharedPtr(SharedPtr<T> &&other) noexcept;
    SharedPtr(const SharedPtr<T> &other) noexcept;
    SharedPtr(std::shared_ptr<T> &&object) noexcept;
    SharedPtr(const std::shared_ptr<T> &object) noexcept;
    SharedPtr(T *object) noexcept;

    template<class Z>
    SharedPtr(SharedPtr<Z> &other);

    SharedPtr(std::unique_ptr<T> && object) noexcept;

    // TODO: remove from class definition
    static auto load (const VersionFileController &version, ReadableAbstract &readable) -> WPair<int, SharedPtr<T>>
    {
        SharedPtr<T> result;
        bool is_value_present;

        if (version.getVersionSharedPtr() != 0)
            return {-1, {}};

        if (readable.read(is_value_present) < 0) {
            return {-1, {}};
        }

        if (not is_value_present)
            return {0, nullptr};

        auto [res, data] = T::loadPtr (version, readable);

        static_assert (std::is_pointer_v<decltype(data)>);

        if (res < 0)
            return {-1, {}};

        result = std::move (data);

        return {0, result};
    }

    /**
     * @return &lt 0 in case Writable fail
     * */
    // TODO: remove from class definition

    template <class Writable> requires(std::is_base_of_v<WritableAbstract, Writable>)
    static auto write (Writable &writable, const SharedPtr<T> &object) -> int
    {
        const bool is_present = object.get() != nullptr;
        const T& ref = *object.get();

        if (writable.write(&is_present, sizeof (is_present)) < 0)
            return -1;

        if (is_present and T::write (writable, ref) < 0) {
            return -1;
        }

        return 0;
    }

    auto operator=(const SharedPtr<T> &other) noexcept -> SharedPtr<T> &;
    auto operator=(SharedPtr<T> &&other) noexcept -> SharedPtr<T> &;

    auto operator=(const SharedPtr<T> &other) const noexcept -> const SharedPtr<T> &;
    auto operator=(SharedPtr<T> &&other) const noexcept -> const SharedPtr<T> &;

    // T const
    template <class Z>
    auto operator=(const SharedPtr<Z> &other) noexcept -> SharedPtr<T> &;
};

template<class T>
inline SharedPtr<T>::SharedPtr(std::unique_ptr<T> &&object) noexcept
    : std::shared_ptr<T>(std::move(object))
{

}

template<class T>
inline SharedPtr<T>::SharedPtr(const SharedPtr<T> &other) noexcept
    : std::shared_ptr<T>(other)
{

}

template<class T>
inline SharedPtr<T>::SharedPtr(SharedPtr<T> &&other) noexcept
    : std::shared_ptr<T>(std::move(other))
{

}

template<class T>
template<class Z>
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

