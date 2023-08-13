#pragma once

#include "utils/WCommonScript.h"
#include <memory>

//#define USE_PRIVATE_UNIQUE_PTR

#ifdef USE_PRIVATE_UNIQUE_PTR
template <class T>
class UniquePtr
{
private:
    T *object;
public:
    UniquePtr() = delete;
    UniquePtr(T *object) noexcept;
    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr(UniquePtr&& other) noexcept;
    UniquePtr(std::nullptr_t) noexcept;
    ~UniquePtr() noexcept;
    
    auto operator->() const -> T*;
    auto operator->() -> T*;
    
    explicit operator bool() const;
    
    auto release() -> void;
    
    auto operator=(const UniquePtr<T>& other) -> UniquePtr<T> & = delete;
    auto operator=(UniquePtr<T>&& other) -> UniquePtr<T> &;
    auto operator==(const UniquePtr<T>& other) const -> bool;
};

template <class T>
inline UniquePtr<T>::~UniquePtr() noexcept
{
    delete this->object;
    this->object = nullptr;
}

template <class T>
inline auto UniquePtr<T>::operator==(const UniquePtr<T>& other) const -> bool
{
    return this->object == other.object;
}

template <class T>
inline auto UniquePtr<T>::release() -> void
{
    delete this->object;
    object = nullptr;
}

template <class T>
inline UniquePtr<T>::UniquePtr(std::nullptr_t) noexcept
    : object(nullptr)
{
    
}

template<class T>
inline UniquePtr<T>::UniquePtr(UniquePtr &&other) noexcept
    : object(other.object)
{
    other.object = nullptr;
}

template <class T>
inline UniquePtr<T>::UniquePtr(T *object) noexcept
    : object(object)
{

}

template <class T>
inline auto UniquePtr<T>::operator->() const -> T *
{
    W_ASSERT(object);
    return this->object;
}

template <class T>
inline auto UniquePtr<T>::operator->() -> T *
{
    W_ASSERT(object);
    return this->object;
}

template <class T>
inline UniquePtr<T>::operator bool() const
{
    return this->object != nullptr;
}

template <class T>
inline UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr &&other)
{
    if (this == &other)
        return *this;
    
    delete object;
    
    this->object = other.object;
    other.object = nullptr;
    
    return *this;
}
#else
template <class T>
using UniquePtr = std::unique_ptr<T>;
#endif // USE_PRIVATE_UNIQUE_PTR
