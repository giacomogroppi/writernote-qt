#include "ExactNumber.h"
#include <numeric>
#include <algorithm>
#include "utils/WCommonScript.h"

auto ExactNumber::operator==(const ExactNumber &other) const -> bool
{
    if (n == ExactInteger(0) and other.n == ExactInteger(0))
        return true;

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

    W_ASSERT(d != ExactInteger(0));

    this->d /= max;
    this->n /= max;
    mod = true;

    tmpValue = this->operator()();

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
    if (this->sign == other.sign)
        sign = false;
    else if (sign != other.sign)
        sign = true;

    W_ASSERT(other.n != ExactInteger(0));

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
    , n (0)
    , d (1)
{
    simplify();
}

auto ExactNumber::operator+=(const ExactNumber &other) -> ExactNumber &
{
    if ((isPositive() and other.isPositive()) or (isNegative() and other.isNegative())) {
        this->n = n * other.d + d * other.n;
        this->d = d * other.d;
        simplify();
        return *this;
    }

    if (*this < other) {
        ExactNumber tmp(other);
        tmp += *this;
        *this = std::move(tmp);
        return *this;
    }

    W_ASSERT(*this >= other);

    const auto shouldKeepSign =
            ((*this > ExactNumber(0)) ? *this : -*this) >
            ((other > ExactNumber(0)) ? other : -other);

    const auto f = n * other.d;
    const auto s = d * other.n;

    if (f > s) {
        n = f - s;
    } else {
        n = s - f;
    }

    if (not shouldKeepSign)
        sign = !sign;

    d = d * other.d;
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
    simplify();
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
    if (value.isEmpty()) {
        return;
    }

    const bool signPresent = value[0] == '+' or value[0] == '-';

    if (value[1] == '-')
        sign = true;

    const auto firstIndex = value.indexOf('.');

    int index;
    if ((index = value.indexOf('.')) != -1) {
        n = value.remove('.').toStdString();
        d = 1;
        for (int i = value.size() - 1; i > index; i--)
            d *= 10;
    } else {
        n = value.toStdString();
        d = 1;
    }

    simplify();
}

ExactNumber::ExactNumber(const ExactNumber::String &num, const ExactNumber::String &den)
    : n("1")
    , d(den.toStdString())
    , sign(false)
{
    if (num[0] == '-' or num[0] == '+') {
        sign = num[0] == '-';
        n = num.mid(1).toStdString();
    } else {
        n = num.toStdString();
    }
    simplify();
}

auto ExactNumber::operator-() const -> ExactNumber
{
    ExactNumber result(*this);
    result.sign = result.sign ? false : true;
    return result;
}

auto ExactNumber::operator<=(const ExactNumber &other) const -> bool
{
    if (n == 0 and other.n == 0)
        return true;
    if (isNegative() and other.isPositive())
        return true;
    if (isPositive() and other.isNegative())
        return false;

    ExactInteger myNum = n * other.d;
    ExactInteger otherNun  = other.n * d;
    return myNum <= otherNun;
}

auto ExactNumber::operator<(const ExactNumber &other) const -> bool
{
    if (isNegative() and other.isPositive())
        return true;
    if (isPositive() and other.isNegative())
        return false;

    ExactInteger myNum = n * other.d;
    ExactInteger otherNum = other.n * d;
    return myNum < otherNum;
}

auto ExactNumber::operator>=(const ExactNumber &other) const -> bool
{
    if (n == 0 and other.n == 0)
        return true;

    if (isNegative() and other.isPositive())
        return false;
    if (isPositive() and other.isNegative())
        return true;

    ExactInteger myNum = n * other.d;
    ExactInteger otherNun  = other.n * d;
    return myNum >= otherNun;
}

auto ExactNumber::operator>(const ExactNumber &other) const -> bool
{
    if (isNegative() and other.isPositive())
        return false;
    if (isPositive() and other.isNegative())
        return true;

    ExactInteger myNum = n * other.d;
    ExactInteger otherNum = other.n * d;
    return myNum > otherNum;
}

auto ExactNumber::operator-(const ExactNumber &other) const -> ExactNumber
{
    ExactNumber result(*this);
    result -= other;
    return result;
}

auto ExactNumber::operator-=(const ExactNumber &other) -> ExactNumber&
{
    (*this) += (-other);
    return *this;
}