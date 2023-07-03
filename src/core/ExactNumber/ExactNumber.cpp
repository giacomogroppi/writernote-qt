#include "ExactNumber.h"
#include <numeric>
#include <algorithm>
#include "utils/WCommonScript.h"

auto ExactNumber::operator==(const ExactNumber &other) const -> bool
{
    return  this->sign == other.sign and
            this->d == other.d and
            this->n == other.n;
}

auto ExactNumber::simplify() -> bool
{
    const auto max = std::gcd(this->d, this->n);

    W_ASSERT(d % max == 0);
    W_ASSERT(n % max == 0);

    this->d /= max;
    this->n /= max;
    return max != 1;
}

auto ExactNumber::operator*=(const ExactNumber &other) -> ExactNumber &
{
    if (this->sign && other.sign)
        sign = false;
    if (sign != other.sign)
        sign = true;

    d *= other.d;
    n *= other.n;

    simplify();

    return *this;
}

auto ExactNumber::operator/=(const ExactNumber &other) -> ExactNumber &
{
    if (this->sign && other.sign)
        sign = false;
    if (sign != other.sign)
        sign = true;

    d *= other.n;
    n *= other.d;

    simplify();

    return *this;
}

ExactNumber::ExactNumber(long numerator, long denominator)
    : d(denominator)
    , n(numerator)
    , sign(false)
{
    if (d < 0 ^ n < 0)
        sign = true;
    simplify();
}


auto ExactNumber::operator/(const ExactNumber &other) const -> ExactNumber
{
    ExactNumber result (*this);
    result /= other;
    return result;
}

auto ExactNumber::operator*(const ExactNumber &other) const -> ExactNumber
{
    ExactNumber result (*this);
    result *= other;
    return result;
}

ExactNumber::ExactNumber()
    : sign(false)
    , n (1)
    , d (0)
{

}

auto ExactNumber::operator+=(const ExactNumber &other) -> ExactNumber &
{
    using highPrecision = long double;

    if (sign != other.sign) {
        // we need to decide the sign
        if (highPrecision (n) / highPrecision (d) > highPrecision(other.n) / highPrecision(d)) {
            // we keep the sign
            n = n * other.d + other.n * d;

            d = d * other.d;
        } else {
            // we change the sign
            n = n * other.d - other.n * d;

            d = d * other.d;
        }
    } else {
        // no problem
        // we keep the sign
        n = n * other.d + other.n * d;

        d = d * other.d;
    }

    simplify();

    return *this;
}

auto ExactNumber::operator+(const ExactNumber &other) const -> ExactNumber
{
    ExactNumber result(*this);
    result += other;
    return result;
}

ExactNumber::ExactNumber(long value)
    : n(value)
    , d(1)
{

}

ExactNumber::ExactNumber(long double number)
    : d(0)
    , n(0)
    , sign(number < 0.)
{
    double tmp;
    int exp = 0;
    double floatPart = 1.;

    while (floatPart != 0.) {
        floatPart =  std::modf(number, &tmp);

        number *= 10.;
        exp ++;
    }

    n = number;

    simplify();
}
