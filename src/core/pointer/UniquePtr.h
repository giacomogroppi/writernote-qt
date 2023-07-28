#pragma once

#include "utils/WCommonScript.h"

template <class T>
class UniquePtr
{
private:
    T *object;
public:
    UniquePtr(T *object) noexcept;
    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr(UniquePtr&& other) noexcept;
    
    auto operator->() const -> T*;
    auto operator->() -> T*;
    
    explicit operator bool() const;
    
    auto operator=(const UniquePtr<T>& other) -> UniquePtr<T> & = delete;
    auto operator=(UniquePtr<T>&& other) -> UniquePtr<T> &;
};

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
    delete object;
    
    this->object = other.object;
    other.object = nullptr;
    
    return *this;
}



