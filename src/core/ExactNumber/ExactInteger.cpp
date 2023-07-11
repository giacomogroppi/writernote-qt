#include "ExactInteger.h"

ExactInteger::ExactInteger(ExactInteger::Digit u)
    : digits(1, u)
{
}

ExactInteger::ExactInteger(const std::string &s)
    : digits(1, 0)
{
    std::istringstream iss(s);
    iss >> *this;
    W_ASSERT(not (iss.fail() || !iss.eof()));
}

ExactInteger::ExactInteger(const ExactInteger &copy)
    : digits(copy.digits)
{}

ExactInteger ExactInteger::operator++(int)
{
    ExactInteger w(*this);
    ++(*this);
    return w;
}

ExactInteger &ExactInteger::operator++()
{
    for (size_t j = 0; j < digits.size() && ++digits[j] == 0; ++j);

    if (digits.back() == 0) {
        digits.push_back(1);
    }

    return *this;
}

ExactInteger& ExactInteger::operator%= (const ExactInteger& rhs)
{
    ExactInteger q;
    divide(rhs, q, *this);
    return *this;
}

void ExactInteger::divide(ExactInteger v, ExactInteger& q, ExactInteger& r) const
{
    // Handle special cases (m < n).
    //W_ASSERT_TEXT (v.digits.back() == 0, "Division by 0" << v.digits);

    r.digits = digits;
    const size_t n = v.digits.size();

    if (digits.size() < n) {
        q.digits.assign(1, 0); return;
    } // Normalize divisor (v[n-1] >= BASE/2).

    unsigned d = BITS;
    for (Digit vn = v.digits.back(); vn != 0; vn >>= 1, --d);
    v <<= d;
    r <<= d;
    const Digit vn = v.digits.back();

    // Ensure first single-digit quotient (u[m-1] < v[n-1]).
    r.digits.push_back(0);
    const size_t m = r.digits.size();
    q.digits.resize(m - n);
    ExactInteger w;
    w.digits.resize(n + 1);
    const Wigit MAX_DIGIT = (static_cast<Wigit>(1) << BITS) - 1;
    for (size_t j = m - n; j-- != 0;)
    {
        // Estimate quotient digit.
        Wigit qhat = std::min(MAX_DIGIT,
                              (static_cast<Wigit>(r.digits[j + n]) << BITS |
                               r.digits[j + n - 1]) / vn);

        // Compute partial product (w = qhat * v).
        Wigit k = 0;
        for (size_t i = 0; i < n; ++i)
        {
            k += qhat * v.digits[i];
            w.digits[i] = static_cast<Digit>(k);
            k >>= BITS;
        }
        w.digits[n] = static_cast<Digit>(k);

        // Check if qhat is too large (u - w < 0).
        bool is_trial = true;
        while (is_trial)
        {
            size_t i = n;
            for (; i != 0 && r.digits[j + i] == w.digits[i]; --i);
            if ((is_trial = (r.digits[j + i] < w.digits[i])))
            {
                // Adjust partial product (w -= v).
                --qhat;
                k = 0;
                for (i = 0; i < n; ++i)
                {
                    k = k + w.digits[i] - v.digits[i];
                    w.digits[i] = static_cast<Digit>(k);
                    k = ((k >> BITS) ? -1 : 0);
                }
                w.digits[n] = static_cast<Digit>(k + w.digits[n]);
            }
        }
        q.digits[j] = static_cast<Digit>(qhat);

        // Compute partial remainder (u -= w).
        k = 0;
        for (size_t i = 0; i < n; ++i)
        {
            k = k + r.digits[j + i] - w.digits[i];
            r.digits[j + i] = static_cast<Digit>(k);
            k = ((k >> BITS) ? -1 : 0);
        }
    }

    // Denormalize remainder.
    q.trim();
    r.digits.resize(n);
    r >>= d;
}

ExactInteger& ExactInteger::operator<<= (size_t rhs)
{
    if (digits.back() != 0 && rhs != 0)
    {
        const size_t n = rhs / BITS;
        digits.insert(digits.begin(), n, 0);
        rhs -= n * BITS;
        Wigit k = 0;
        for (size_t j = n; j < digits.size(); ++j)
        {
            k |= static_cast<Wigit>(digits[j]) << rhs;
            digits[j] = static_cast<Digit>(k);
            k >>= BITS;
        }
        if (k != 0)
        {
            digits.push_back(static_cast<Digit>(k));
        }
    }
    return *this;
}

ExactInteger& ExactInteger::operator>>= (size_t rhs)
{
    const size_t n = rhs / BITS;
    if (n >= digits.size())
    {
        digits.assign(1, 0);
    }
    else
    {
        digits.erase(digits.begin(), digits.begin() + n);
        rhs -= n * BITS;
        Wigit k = 0;
        for (size_t j = digits.size(); j-- != 0;)
        {
            k = k << BITS | digits[j];
            digits[j] = static_cast<Digit>(k >> rhs);
            k = static_cast<Digit>(k);
        }
        trim();
    }
    return *this;
}

ExactInteger& ExactInteger::operator*= (const ExactInteger& rhs)
{
    *this = (*this) * rhs;
    return *this;
}

ExactInteger& ExactInteger::operator&= (const ExactInteger& rhs)
{
    const size_t n = rhs.digits.size();
    if (digits.size() > n)
    {
        digits.resize(n);
    }
    for (size_t j = 0; j < digits.size(); ++j)
    {
        digits[j] &= rhs.digits[j];
    }
    trim();
    return *this;
}

ExactInteger& ExactInteger::operator^= (const ExactInteger& rhs)
{
    const size_t n = rhs.digits.size();
    if (digits.size() < n)
    {
        digits.resize(n, 0);
    }
    for (size_t j = 0; j < n; ++j)
    {
        digits[j] ^= rhs.digits[j];
    }
    trim();
    return *this;
}

ExactInteger& ExactInteger::operator+= (const ExactInteger& rhs)
{
    const size_t n = rhs.digits.size();
    if (digits.size() < n)
    {
        digits.resize(n, 0);
    }
    size_t j = 0;
    Wigit k = 0;
    for (; j < n; ++j)
    {
        k = k + digits[j] + rhs.digits[j];
        digits[j] = static_cast<Digit>(k);
        k >>= BITS;
    }
    for (; k != 0 && j < digits.size(); ++j)
    {
        k += digits[j];
        digits[j] = static_cast<Digit>(k);
        k >>= BITS;
    }
    if (k != 0)
    {
        digits.push_back(1);
    }
    return *this;
}

ExactInteger& ExactInteger::operator-= (const ExactInteger& rhs)
{
    W_ASSERT_TEXT((*this) >= rhs, *this << "is less than" << rhs);
    size_t j = 0;
    Wigit k = 0;
    for (; j < rhs.digits.size(); ++j)
    {
        k = k + digits[j] - rhs.digits[j];
        digits[j] = static_cast<Digit>(k);
        k = ((k >> BITS) ? -1 : 0);
    }
    for (; k != 0 && j < digits.size(); ++j)
    {
        k += digits[j];
        digits[j] = static_cast<Digit>(k);
        k = ((k >> BITS) ? -1 : 0);
    }
    trim();
    return *this;
}

ExactInteger& ExactInteger::operator/= (const ExactInteger& rhs)
{
    ExactInteger r;
    divide(rhs, *this, r);
    return *this;
}

ExactInteger& ExactInteger::operator|= (const ExactInteger& rhs)
{
    const size_t n = rhs.digits.size();
    if (digits.size() < n)
    {
        digits.resize(n, 0);
    }
    for (size_t j = 0; j < n; ++j)
    {
        digits[j] |= rhs.digits[j];
    }
    return *this;
}

ExactInteger& ExactInteger::operator-- ()
{
    if (digits.back() == 0)
    {
        throw std::underflow_error("Error: ExactInteger::underflow");
    }
    for (size_t j = 0; j < digits.size() && digits[j]-- == 0; ++j);
    trim();
    return *this;
}

std::string ExactInteger::to_string() const
{
    std::ostringstream oss;
    ExactInteger q(*this), r;
    do
    {
        q.divide(10, q, r);
        oss << r.digits[0];
    } while (q.digits.back() != 0);
    std::string s(oss.str());
    std::reverse(s.begin(), s.end());
    return s;
}

ExactInteger::Digit ExactInteger::to_uint() const
{
    return digits[0];
}

ExactInteger ExactInteger::gcd(const ExactInteger& other) const
{
    ExactInteger a(*this);
    ExactInteger b(other);

    while (b != ExactInteger("0")) {
        ExactInteger temp(b);
        b = a % b;
        a = temp;
    }

    return a;
}

ExactInteger ExactInteger::operator-- (int)
{
    ExactInteger w(*this);
    --(*this);
    return w;
}

void ExactInteger::trim()
{
    while (digits.back() == 0 && digits.size() > 1)
    {
        digits.pop_back();
    }
}
