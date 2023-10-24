#pragma once

#include "SharedPtrThreadSafe.h"

template <class T>
class Pointer
{
private:
#ifdef DEBUGINFO
    SharedPtrThreadSafe<T> _pointer;
#else
    T* _object;
#endif // DEBUGINFO
public:
    explicit Pointer(T* object = nullptr);
    ~Pointer();

    Pointer(Pointer&& other) noexcept;
    Pointer(const Pointer& other) noexcept;

#ifdef DEBUGINFO
    auto numberOfRef() const -> int;
#endif // DEBUGINFO

    /**
     * \brief This method will destroy the object
     * */
    void release();

    auto operator=(Pointer&& other) -> Pointer&;
    auto operator=(const Pointer& other) -> Pointer&;

    auto operator->() const -> const T*;
    auto operator*() const -> const T&;

    auto operator->() -> T*;
    auto operator*() -> T&;

    void doAndUnref (auto method);

    auto operator==(const Pointer<T>& other) const -> bool;

    explicit operator bool() const;
};

template <class T>
auto Pointer<T>::operator==(const Pointer<T> &other) const -> bool
{
    return _pointer == other._pointer;
}

template <class T>
void Pointer<T>::doAndUnref(auto method)
{
    W_ASSERT(_pointer.operator bool());

    this->_pointer.doAndUnref(method);
}

template <class T>
inline Pointer<T>::operator bool() const
{
    return _pointer.operator bool();
}

template<class T>
inline auto Pointer<T>::operator=(const Pointer &other) -> Pointer &
{
    if (this == &other)
        return *this;

    release();

    this->_pointer = other._pointer;

    return *this;
}

template<class T>
inline auto Pointer<T>::operator=(Pointer &&other) -> Pointer &
{
    if (this == &other)
        return *this;

    release();

    this->_pointer = std::move(other._pointer);

    return *this;
}

template <class T>
inline Pointer<T>::Pointer(T* object)
    : _pointer(object)
{
}

template <class T>
inline Pointer<T>::~Pointer()
{
    if (_pointer.get() != nullptr) {
        W_ASSERT(not _pointer.isUnique());
    }
}

template <class T>
inline void Pointer<T>::release()
{
    W_ASSERT(_pointer.isUnique());
    _pointer = SharedPtrThreadSafe<T>(nullptr);
}

template <class T>
inline auto Pointer<T>::operator->() const -> const T*
{
    return _pointer.get();
}

template <class T>
inline auto Pointer<T>::operator*() const -> const T&
{
    return *_pointer;
}

template <class T>
inline auto Pointer<T>::operator->() -> T*
{
    return _pointer.get();
}

template <class T>
inline auto Pointer<T>::operator*() -> T&
{
    return *_pointer;
}

template <class T>
inline Pointer<T>::Pointer(Pointer&& other) noexcept
    : _pointer(std::move(other._pointer))
{

}

template <class T>
inline Pointer<T>::Pointer(const Pointer& other) noexcept
    : _pointer(other._pointer)
{

}

#ifdef DEBUGINFO
template <class T>
auto Pointer<T>::numberOfRef() const -> int
{
    return _pointer.numberOfRef();
}
#endif // DEBUGINFO
