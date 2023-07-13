#pragma once

#include <cstdio>
#include "utils/WCommonScript.h"
#include "ExactInteger.h"
#include "core/String/WString.h"

class ExactNumber {
private:
    using String = WString;
    /**
     * true if negative
     * */
    bool sign: 1;
    ExactInteger n;
    ExactInteger d;

    std::string tmpValue;

    auto simplify() -> bool;

public:
    ExactNumber(int value);
    ExactNumber(const String& value);
    explicit ExactNumber(const String& num, const String& den);
    explicit ExactNumber(long numerator, long denominator);
    explicit ExactNumber(long double number);
    ExactNumber();
    ExactNumber(const ExactNumber &other) noexcept = default;
    ExactNumber(ExactNumber &&other) noexcept = default;

    auto isPositive() const -> bool;
    auto isNegative() const -> bool;

    auto operator+=(const ExactNumber &other) -> ExactNumber&;
    auto operator*=(const ExactNumber &other) -> ExactNumber&;
    auto operator-=(const ExactNumber &other) -> ExactNumber&;
    auto operator/=(const ExactNumber &other) -> ExactNumber&;

    auto operator+(const ExactNumber &other) const -> ExactNumber;
    auto operator-(const ExactNumber &other) const -> ExactNumber;
    auto operator/(const ExactNumber &other) const -> ExactNumber;
    auto operator*(const ExactNumber &other) const -> ExactNumber;

    auto operator==(const ExactNumber &other) const -> bool;
    auto operator>=(const ExactNumber &other) const -> bool;
    auto operator>(const ExactNumber& other) const -> bool;
    auto operator<(const ExactNumber& other) const -> bool;
    auto operator<=(const ExactNumber& other) const -> bool;

    auto operator-() const -> ExactNumber;

    auto operator=(const ExactNumber &other) noexcept -> ExactNumber& = default;
    auto operator=(ExactNumber &&other) noexcept -> ExactNumber& = default;

    auto operator()() const -> std::string;

#ifdef USE_QT
    friend QDebug operator<<(QDebug d, const ExactNumber &p);
#endif // USE_QT
};

inline auto ExactNumber::operator()() const -> std::string
{
    return (isPositive() ? "+" : "-") + this->n.to_string() + "/" + this->d.to_string();
}

inline auto ExactNumber::isNegative() const -> bool
{
    return sign == 1;
}

inline auto ExactNumber::isPositive() const -> bool
{
    return !isNegative();
}

#ifdef USE_QT
inline Q_CORE_EXPORT QDebug operator<<(QDebug d, const ExactNumber &p)
{
    return d.nospace() << "(" << (p.sign == true ? "-" : "+") << p.n << "/" << p.d << ")";
}
#endif // USE_QT