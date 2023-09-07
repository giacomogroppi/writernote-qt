#pragma once

#include <QPainter>

template <class Enum>
class WFlags
{
private:
    using Type = unsigned long;
    Type _d;
public:
    WFlags() noexcept : _d(0) {}
    WFlags(const Enum &e) noexcept : _d(Type(e)) {};

    WFlags(WFlags &&other) = default;
    WFlags(const WFlags &other) = default;

    auto operator&(Enum value) const -> WFlags { WFlags result; result._d = Type(value) & _d; return result; }
    auto operator|(Enum value) const -> WFlags { WFlags result; result._d = Type(value) | _d; return result; }

    auto operator&(WFlags value) const -> WFlags { WFlags result; result._d = value._d & _d; return result; }
    auto operator|(WFlags value) const -> WFlags { WFlags result; result._d = value._d | _d; return result; }

    friend auto operator&(WFlags<Enum> f1, WFlags<Enum> &f2) -> WFlags<Enum>
    {
        WFlags<Enum> result;
        result._d = f1._d & f2._d;
        return result;
    };

    friend auto operator|(WFlags f1, WFlags &f2) -> WFlags
    {
        WFlags<Enum> result;
        result._d = f1._d | f2._d;
        return result;
    };

    /*friend auto operator|(WFlags<Enum> f1, Enum value) -> WFlags<Enum>
    {
        WFlags<Enum> result;
        result._d = f1._d | Type(value);
        return result;
    }*/

    auto operator&=(const WFlags<Enum> &d) -> WFlags<Enum>& { _d &= d._d; return *this; }
    auto operator|=(const WFlags<Enum> &d) -> WFlags<Enum>& { _d |= d._d; return *this; }

    auto operator=(const WFlags<Enum> &d) -> WFlags<Enum>& { _d = d._d; return *this;}

    auto operator=(WFlags<Enum> &&d) -> WFlags<Enum>&
    {
        this->operator=(std::as_const(d));
        return *this;
    };

    operator bool () const { return _d != 0; }
};

template <class T>
inline WFlags<T> operator|(const WFlags<T> &f1, const WFlags<T> &f2)
{
    return f1.operator|(f2);
}

template <class T>
inline WFlags<T> operator|(const T& f1, const T& f2)
{
    return WFlags<T>(f1).operator|(f2);
}

template <class T>
inline WFlags<T> operator|(const WFlags<T>& f1, const T& f2)
{
    return f1.operator|(f2);
}
