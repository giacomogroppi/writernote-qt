#pragma once

#include <utility>
#include <initializer_list>

template<typename Enum>
    requires (sizeof(Enum) < sizeof(unsigned long) && std::is_enum_v<Enum>)
class WFlags
{
    typedef typename std::conditional_t<
        std::is_signed<Enum>::value,
            signed long,
            unsigned long
        > Type;
    Type _d;
public:
    constexpr WFlags(Enum f) : _d(Type(f)) {}
    constexpr WFlags() : _d(0) {}

    template <class Z> requires ((std::is_same<Z, Type>::value and not std::is_same<Z, Enum>::value)
                                or std::is_same<Z, int>::value)
    constexpr WFlags(Z f) : _d(f) {}

    constexpr auto operator&=(Type mask) -> WFlags<Enum> & { _d &= mask; return *this; }
    constexpr auto operator|=(WFlags<Enum> f) -> WFlags<Enum> &{ _d |= f._d; return *this; }

    constexpr operator Type() const { return _d; }

    constexpr auto operator|(WFlags<Enum> f) const -> WFlags<Enum> { return Type(_d | f._d); }
    consteval auto operator|(Enum value) const -> WFlags<Enum> { return Type(_d | value); };

    constexpr auto operator&(WFlags<Enum> f) const -> WFlags<Enum> { return Type(_d & f._d); }
    constexpr auto operator&(Enum value) const -> WFlags<Enum> { return Type(_d & value); };

    template <class T = Type>
        requires (std::is_arithmetic_v<T> and sizeof(T) < sizeof(Type))
    constexpr auto operator&(T value) const -> WFlags { return Type (_d & value); }

    template <class T = Type>
        requires (std::is_arithmetic_v<T> and sizeof(T) < sizeof(Type))
    constexpr auto operator|(T value) const -> WFlags { return Type (_d | value); }

    constexpr auto operator~() const -> WFlags<Enum> { return Type(~_d); }
};
