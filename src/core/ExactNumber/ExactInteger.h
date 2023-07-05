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
};
