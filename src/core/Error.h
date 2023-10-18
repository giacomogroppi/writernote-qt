#pragma once

#include "utils/WCommonScript.h"

class Error
{
protected:
    int value;

    explicit Error(int value);
public:
    Error(const Error& other) noexcept = default;

    nd auto isErr() const -> bool;

#define DefineErr(name, v) \
    nd static auto make##name() -> Error { return Error{v}; }; \
    nd auto is##name() const -> bool { return this->value == (v); }

    DefineErr(Ok, 0);
    DefineErr(ErrVersion, -1);
    DefineErr(ErrInternal, -2);
    DefineErr(Corruption, -3);
    DefineErr(ErrFileNotExist, -4);

    operator bool () const;

    auto operator=(const Error& error) noexcept -> Error& = default;
};

inline Error::operator bool() const
{
    return isErr();
}

inline Error::Error(int value)
    : value(value)
{

}

inline auto Error::isErr() const -> bool
{
    return value < 0;
}
