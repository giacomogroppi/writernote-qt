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
    ExactInteger max("0");
    bool mod = false;

    max = this->d.gcd(n);

    W_ASSERT(d % max == 0);
    W_ASSERT(n % max == 0);

    this->d /= max;
    this->n /= max;
    mod = true;

    return mod;
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
    if (sign != other.sign) {
        // we need to decide the sign
        if (n / d > other.n / d) {
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

ExactNumber::ExactNumber(int value)
    : n(value)
    , d(1)
    , sign(value < 0)
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

ExactNumber::ExactNumber(const ExactNumber::String &value)
    : d(1)
    , n(0)
    , sign(false)
{
    WString num, den;

    if (value.isEmpty()) {
        return;
    }

    const bool signPresent = value[0] == '+' or value[0] == '-';

    if (value[1] == '-')
        sign = true;

    const auto firstIndex = value.indexOf('.');

    WString numerator;
    for (int i = value.size() - 1; i >= 0; i--) {
        if (value[i] == '.' or value[i] == ',')
            continue;
        numerator.append(value[i]);
    }

    n = num.toStdString();
    d = den.toStdString();

    n *= std::pow(10, den.size() - 1);
    d *= std::pow(10, den.size() - 1);

    simplify();
}

ExactNumber::ExactNumber(const ExactNumber::String &num, const ExactNumber::String &den)
    : n(num.toStdString())
    , d(den.toStdString())
    , sign(false)
{

}
