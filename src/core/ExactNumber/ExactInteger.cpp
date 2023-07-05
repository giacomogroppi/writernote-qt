#include "ExactInteger.h"

ExactInteger::ExactInteger(int number)
    : _digits()
{
    const auto numberOfDigit = WCommonScript::ecc(std::log10(number));
    _digits.reserve(numberOfDigit);

    for (int i = 0; i < numberOfDigit; i++) {
        const int pow = std::pow(10, numberOfDigit - i - 1);
        const auto digit = (number / pow) % 10;
        addDigit(digit);
    }
}

auto ExactInteger::operator-=(const ExactInteger &other) -> ExactInteger &
{
    int borrow = 0;
    int maxDigits = std::max(_digits.size(), other._digits.size());

    for (int i = 0; i < maxDigits; ++i) {
        int digitDiff = borrow;

        if (i < _digits.size())
            digitDiff += _digits[i];

        if (i < other._digits.size())
            digitDiff -= other._digits[i];

        if (digitDiff < 0) {
            digitDiff += 10;
            borrow = -1;
        } else {
            borrow = 0;
        }

        addDigit(digitDiff);
    }

    // Rimuovi le eventuali cifre iniziali zero
    while (_digits.size() > 1 && _digits.back() == 0) {
        _digits.pop_back();
    }

    return *this;
}

auto ExactInteger::operator/=(const ExactInteger &other) -> ExactInteger &
{
    if (other._digits.size() == 1 && other._digits[0] == 0) {
        std::cout << "Errore: divisione per zero." << std::endl;
        return *this;
    }

    ExactInteger quotient;
    ExactInteger dividend(*this);

    while (dividend._digits.size() > 0 && dividend.isGreaterThan(other)) {
        int currentDigit = 0;
        ExactInteger partialDividend;

        while (partialDividend.isLessThanOrEqualTo(other) && currentDigit < 10) {
            partialDividend = partialDividend.multiplyBy10();
            partialDividend = partialDividend + dividend._digits.back();
            dividend._digits.pop_back();
            ++currentDigit;
        }

        currentDigit--;

        while (currentDigit >= 0) {
            int quotientDigit = 0;

            while (partialDividend.isLessThanOrEqualTo(other)) {
                partialDividend = partialDividend - other;
                ++quotientDigit;
            }

            quotient.addDigit(quotientDigit);

            partialDividend = partialDividend.multiplyBy10();
            partialDividend = partialDividend + dividend._digits.back();
            dividend._digits.pop_back();

            --currentDigit;
        }
    }

    std::reverse(quotient._digits.begin(), quotient._digits.end());
    *this = std::move(quotient);
    return *this;
}

auto ExactInteger::digitAt(int index) const -> int
{
    return this->_digits[index];
}

auto ExactInteger::operator+=(const ExactInteger &other) -> ExactInteger &
{
    int carry = 0;
    int maxDigits = std::max(_digits.size(), other._digits.size());

    for (int i = 0; i < maxDigits; ++i) {
        int digitSum = carry;

        if (i < _digits.size())
            digitSum += _digits[i];

        if (i < other._digits.size())
            digitSum += other._digits[i];

        carry = digitSum / 10;
        digitSum %= 10;

        addDigit(digitSum);
    }

    if (carry > 0)
        addDigit(carry);

    return *this;
}

auto ExactInteger::addDigit(int digit) -> ExactInteger &
{
    W_ASSERT(digit >= 0 and digit <= 9);
    _digits.push_back(digit);
    return *this;
}

auto ExactInteger::operator*=(const ExactInteger &other) -> ExactInteger &
{
    ExactInteger result;
    std::vector<int> partialSums;

    for (int i = 0; i < _digits.size(); ++i) {
        int carry = 0;

        for (int j = 0; j < other._digits.size(); ++j) {
            int partialProduct = _digits[i] * other._digits[j] + carry;

            if (i + j < partialSums.size())
                partialProduct += partialSums[i + j];

            carry = partialProduct / 10;
            partialProduct %= 10;

            if (i + j < partialSums.size())
                partialSums[i + j] = partialProduct;
            else
                partialSums.push_back(partialProduct);
        }

        if (carry > 0)
            partialSums.push_back(carry);
    }

    *this = std::move(partialSums);
    return *this;
}

auto ExactInteger::operator+(int number) const -> ExactInteger
{
    ExactInteger result(*this);
    result += ExactInteger(number);
    return result;
}

auto ExactInteger::operator-(int number) const -> ExactInteger
{
    ExactInteger result(*this);
    result -= number;
    return result;
}

auto ExactInteger::operator*(int number) const -> ExactInteger
{
    ExactInteger result(*this);
    result *= number;
    return result;
}

auto ExactInteger::operator/(int number) const -> ExactInteger
{
    ExactInteger result(*this);
    result /= number;
    return result;
}

auto ExactInteger::operator+(const ExactInteger& number) const -> ExactInteger
{
    ExactInteger result(*this);
    result += ExactInteger(number);
    return result;
}

auto ExactInteger::operator-(const ExactInteger& number) const -> ExactInteger
{
    ExactInteger result(*this);
    result -= number;
    return result;
}

auto ExactInteger::operator*(const ExactInteger& number) const -> ExactInteger
{
    ExactInteger result(*this);
    result *= number;
    return result;
}

auto ExactInteger::operator/(const ExactInteger& number) const -> ExactInteger
{
    ExactInteger result(*this);
    result /= number;
    return result;
}

auto ExactInteger::isLessThanOrEqualTo(const ExactInteger &other) const -> bool
{
    return !isGreaterThan(other);
}

auto ExactInteger::isGreaterThan(const ExactInteger &other) const -> bool
{
    if (_digits.size() > other._digits.size())
        return true;
    if (_digits.size() < other._digits.size())
        return false;

    for (int i = _digits.size() - 1; i >= 0; --i) {
        if (_digits[i] > other._digits[i])
            return true;
        if (_digits[i] < other._digits[i])
            return false;
    }

    return false;
}

auto ExactInteger::multiplyBy10() const -> ExactInteger
{
    ExactInteger result(*this);
    result.addDigit(0);
    return result;
}

ExactInteger::ExactInteger(std::vector<int> digits)
    : _digits(std::move(digits))
{

}

ExactInteger::ExactInteger(const std::string& str)
    : _digits()
{
    for (char c: str) {
        W_ASSERT(c >= '0' and c <= '9');
        const int realDigit = c - '0';
        addDigit(realDigit);
    }
}

auto ExactInteger::operator==(const ExactInteger &other) const -> bool
{
    return _digits == other._digits;
}
