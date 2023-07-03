#pragma once

#include <cstdio>
#include "utils/WCommonScript.h"

class ExactNumber {
private:
    /**
     * true if negative
     * */
    bool sign: 1;
    unsigned long n;
    unsigned long d;

    auto simplify() -> bool;

public:
    explicit ExactNumber(long value);
    explicit ExactNumber(long numerator, long denominator);
    explicit ExactNumber(long double number);
    ExactNumber();

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

    operator size_t () const { return n / d; }

    friend QDebug operator<<(QDebug d, const ExactNumber &p);
};

inline auto ExactNumber::isNegative() const -> bool
{
    return sign == 1;
}

#ifdef USE_QT
inline Q_CORE_EXPORT QDebug operator<<(QDebug d, const ExactNumber &p)
{
    return d.nospace() << "ExactNumber(" << (p.sign == true ? "-" : "+") << p.n << "/" << p.d << ")";
}
#endif // USE_QT