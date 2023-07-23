#pragma once

#include "ExactNumber.h"
#include "core/WVector.h"

class ComplexNumberPrivate
{
private:
    ExactInteger number;
    ExactInteger exp;
    bool isNegative;
public:
    ComplexNumberPrivate (ExactInteger exactInteger);
    ComplexNumberPrivate () = default;
    ComplexNumberPrivate (const ComplexNumberPrivate&) noexcept = default;
    ComplexNumberPrivate (ComplexNumberPrivate&&) noexcept = default;



    auto operator=(const ComplexNumberPrivate &) noexcept -> ComplexNumberPrivate& = default;
    auto operator=(ComplexNumberPrivate &&) noexcept -> ComplexNumberPrivate& = default;
};

class ComplexNumber
{
private:
    WVector<ComplexNumberPrivate> numbers;
public:
    ComplexNumber (ExactInteger exactInteger);
    ComplexNumber () = default;
    ComplexNumber (const ComplexNumber&) noexcept = default;
    ComplexNumber (ComplexNumber&&) noexcept = default;

    auto operator*(const ComplexNumber &other) const noexcept -> ComplexNumber;
    auto operator+(const ComplexNumber &other) const noexcept -> ComplexNumber;
    auto operator/(const ComplexNumber &other) const noexcept -> ComplexNumber;
    auto operator-(const ComplexNumber &other) const noexcept -> ComplexNumber;

    auto operator=(const ComplexNumber &) noexcept -> ComplexNumber& = default;
    auto operator=(ComplexNumber &&) noexcept -> ComplexNumber& = default;
};