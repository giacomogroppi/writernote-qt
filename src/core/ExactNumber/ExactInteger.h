#pragma once

#include <vector>
#include "utils/WCommonScript.h"

class ExactInteger
{
    using Vector = std::vector<int>;
private:
    Vector _digits;
public:
    ExactInteger() = default;
    ExactInteger(std::vector<int> digits);
    ExactInteger(const std::string& str);
    ExactInteger(int number);
    ExactInteger(const ExactInteger &other) = default;
    ExactInteger(ExactInteger &&other) = default;
    auto operator+= (const ExactInteger &other) -> ExactInteger &;

    auto addDigit(int digit) -> ExactInteger&;

    auto operator*=(const ExactInteger& other) -> ExactInteger &;
    auto operator-=(const ExactInteger &other) -> ExactInteger&;
    auto operator/=(const ExactInteger &other) -> ExactInteger&;

    auto operator+(int number) const -> ExactInteger;
    auto operator-(int number) const -> ExactInteger;
    auto operator*(int number) const -> ExactInteger;
    auto operator/(int number) const -> ExactInteger;

    auto operator+(const ExactInteger& number) const -> ExactInteger;
    auto operator-(const ExactInteger& number) const -> ExactInteger;
    auto operator*(const ExactInteger& number) const -> ExactInteger;
    auto operator/(const ExactInteger& number) const -> ExactInteger;

    auto isLessThanOrEqualTo(const ExactInteger &other) const -> bool;
    auto isGreaterThan(const ExactInteger &other) const -> bool;

    auto multiplyBy10() const -> ExactInteger;

    auto operator=(const ExactInteger &other) -> ExactInteger& = default;
    auto operator=(ExactInteger &&other) -> ExactInteger& = default;

    auto digitAt(int index) const -> int;

    auto operator==(const ExactInteger &other) const -> bool;

    friend QDebug operator<<(QDebug d, const ExactInteger &p);

};

#ifdef USE_QT
inline Q_CORE_EXPORT QDebug operator<<(QDebug d, const ExactInteger &p)
{
    d.space() << "ExactInteger(";
    for (const auto &i: p._digits) {
        d.nospace() << i;
    }
    return d.nospace() << ")";
}
#endif // USE_QT
