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

    template <class Z> requires (not std::is_pointer<Z>::value and std::is_convertible<Z*, T*>::value)
    explicit Pointer(Z* data);

    template <class Z> requires (not std::is_pointer<Z>::value and std::is_convertible<Z*, T*>::value)
    Pointer(const Pointer<Z> &data);

    template <class Z> requires (not std::is_pointer<Z>::value and std::is_convertible<Z*, T*>::value)
    Pointer(Pointer<Z> &&data);

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

    template <class Z> requires (not std::is_pointer<Z>::value and std::is_convertible<Z*, T*>::value)
    auto operator=(const Pointer<Z> &data) -> Pointer<T>&;

    template <class Z> requires (not std::is_pointer<Z>::value and std::is_convertible<Z*, T*>::value)
    auto operator=(Pointer<Z> &&data) -> Pointer<T>&;


    auto operator=(Pointer&& other) -> Pointer&;
    auto operator=(const Pointer& other) -> Pointer&;

    auto operator->() const -> const T*;
    auto operator*() const -> const T&;

    auto operator->() -> T*;
    auto operator*() -> T&;

    void doAndUnref (Fn<void(T&)> method);

    auto operator==(const Pointer<T>& other) const -> bool;

    template<class ...Args>
    static Pointer<T> make(Args &&...args)
    {
        return Pointer<T>(new T(std::forward<Args>(args)...));
    }

    explicit operator bool() const;

    template <class Z>
    friend class Pointer;
};

template <class T>
template <class Z> requires (not std::is_pointer<Z>::value and std::is_convertible<Z*, T*>::value)
Pointer<T>::Pointer(const Pointer<Z> &data)
    : _pointer(data._pointer)
{

}

template <class T>
template <class Z> requires (not std::is_pointer<Z>::value and std::is_convertible<Z*, T*>::value)
Pointer<T>::Pointer(Pointer<Z> &&data)
    : _pointer(std::move(data._pointer))
{

}

template <class T>
auto Pointer<T>::operator==(const Pointer<T> &other) const -> bool
{
    return _pointer == other._pointer;
}

template <class T>
void Pointer<T>::doAndUnref(Fn<void(T&)> method)
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
Pointer<T>::Pointer(T* object)
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
Pointer<T>::Pointer(Pointer&& other) noexcept
    : _pointer(std::move(other._pointer))
{

}

template <class T>
Pointer<T>::Pointer(const Pointer& other) noexcept
    : _pointer(other._pointer)
{

}

template <class T>
template <class Z> requires (not std::is_pointer<Z>::value and std::is_convertible<Z*, T*>::value)
Pointer<T>::Pointer(Z* data)
    : _pointer(data)
{

}

#ifdef DEBUGINFO
template <class T>
auto Pointer<T>::numberOfRef() const -> int
{
    return _pointer.numberOfRef();
}
#endif // DEBUGINFO
