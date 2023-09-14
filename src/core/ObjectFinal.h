#pragma once

#include <memory>
#include "utils/WCommonScript.h"

template <class Object>
class ObjectFinal
{
private:
#ifdef DEBUGINFO
    bool _write = false;
#endif // DEBUGINFO
    Object _object;

public:
    ObjectFinal() : _write(false), _object() {};

    explicit ObjectFinal(Object &&object) : _write(true), _object(std::move(object)) {};
    explicit ObjectFinal(const Object &object) : _write(true), _object(object) {};

    auto operator->() const -> const Object&;
    auto operator->() -> Object&;

    auto operator=(const ObjectFinal &object) -> ObjectFinal& = delete;
    auto operator=(ObjectFinal &&object) -> ObjectFinal& = delete;

    auto operator=(const Object &object) -> ObjectFinal&;
    auto operator=(Object &&object) -> ObjectFinal&;
};

template<class Object>
inline auto ObjectFinal<Object>::operator->() const -> const Object &
{
    W_ASSERT(_write == true);
    return _object;
}

template<class Object>
inline auto ObjectFinal<Object>::operator->() -> Object &
{
    W_ASSERT(_write == true);
    return _object;
}

template<class Object>
auto ObjectFinal<Object>::operator=(Object &&object) -> ObjectFinal &
{
    W_ASSERT(_write == false);

#ifdef DEBUGINFO
    _write = true;
#endif // DEBUGINFO
    _object = std::move(object);

    return *this;
}

template<class Object>
inline auto ObjectFinal<Object>::operator=(const Object &object) -> ObjectFinal &
{
    W_ASSERT(_write == false);

#ifdef DEBUGINFO
    _write = true;
#endif // DEBUGINFO
    _object = object;

    return *this;
}


