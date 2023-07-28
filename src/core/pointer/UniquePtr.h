#pragma once


#include <memory>

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
    
    UniquePtr<T> &operator=(const UniquePtr<T>& other);
    UniquePtr<T> &operator=(UniquePtr<T>&& other);
};

template <class T>
inline UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr &&other)
{
    delete object;
    
    this->object = other.object;
    other.object = nullptr;
    
    return *this;
}



