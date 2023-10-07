#pragma once

#include "core/WElement.h"

class Index
{
private:
    long _index;

    Index() = default;

public:
    /**
     * \param value It's need to be > 0
     * */
    constexpr Index (long value) noexcept;
    static auto constexpr makeInvalid() noexcept -> Index;

    nd constexpr auto isValid() const noexcept -> bool;
    nd constexpr auto isInvalid() const noexcept -> bool;

    constexpr explicit operator long() const;

    constexpr auto value() const noexcept -> long;

    constexpr auto operator ++() noexcept -> Index&;
    constexpr auto operator --() noexcept -> Index&;

    constexpr auto operator--(int) -> Index;

    constexpr auto operator>(Index other) const noexcept -> bool;
    constexpr auto operator>=(Index other) const noexcept -> bool;
    constexpr auto operator<(Index other) const noexcept -> bool;
    constexpr auto operator<=(Index other) const noexcept -> bool;

    constexpr auto operator==(const Index& other) const -> bool= default;
    constexpr auto operator!=(const Index& other) const -> bool = default;
};

inline constexpr auto Index::value() const noexcept -> long
{
    return this->_index;
}

inline auto constexpr Index::makeInvalid() noexcept -> Index
{
    Index index{};
    index._index = -1;
    return index;
}

inline constexpr Index::operator long() const
{
    W_ASSERT(isValid());
    return _index;
}

inline auto constexpr Index::isValid() const noexcept -> bool
{
    return _index >= 0;
}

inline auto constexpr Index::isInvalid() const noexcept -> bool
{
    return not isValid();
}

inline constexpr Index::Index(long value) noexcept
        : _index(value)
{

}

inline constexpr auto Index::operator ++() noexcept -> Index&
{
    _index ++;
    return *this;
}

inline constexpr auto Index::operator --() noexcept -> Index&
{
    _index --;
    return *this;
}

inline constexpr auto Index::operator--(int) -> Index
{
    Index tmp(*this);
    _index --;
    return tmp;
}

inline constexpr auto Index::operator>(Index other) const noexcept -> bool
{
    return this->_index > other._index;
}

inline constexpr auto Index::operator>=(Index other) const noexcept -> bool
{
    return this->_index >= other._index;
}

inline constexpr auto Index::operator<(Index other) const noexcept -> bool
{
    return this->_index < other._index;
}

inline constexpr auto Index::operator<=(Index other) const noexcept -> bool
{
    return this->_index <= other._index;
}
