#pragma once

#include <cstdint>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <cctype>
using std::size_t;

#include "utils/WCommonScript.h"
#include <QDebug>

class ExactInteger
{
public:
    typedef std::uint32_t Digit;
    typedef std::uint64_t Wigit;
    static const unsigned BITS = 32;

    ExactInteger(Digit u = 0);
    ExactInteger(const std::string& s);
    ExactInteger(const ExactInteger& copy);

    ExactInteger operator++ (int);
    ExactInteger& operator++ ();
    ExactInteger operator-- (int);
    ExactInteger& operator-- ();

    ExactInteger& operator+= (const ExactInteger& rhs);
    ExactInteger& operator-= (const ExactInteger& rhs);
    ExactInteger& operator*= (const ExactInteger& rhs);
    ExactInteger& operator/= (const ExactInteger& rhs);
    ExactInteger& operator%= (const ExactInteger& rhs);

    void divide(ExactInteger v, ExactInteger& q, ExactInteger& r) const;

    ExactInteger& operator<<= (size_t rhs);
    ExactInteger& operator>>= (size_t rhs);
    ExactInteger& operator&= (const ExactInteger& rhs);
    ExactInteger& operator^= (const ExactInteger& rhs);
    ExactInteger& operator|= (const ExactInteger& rhs);

    friend ExactInteger operator& (const ExactInteger& u, const ExactInteger& v);
    friend ExactInteger operator% (const ExactInteger& u, const ExactInteger& v);
    friend ExactInteger operator+ (const ExactInteger& u, const ExactInteger& v);
    friend ExactInteger operator- (const ExactInteger& u, const ExactInteger& v);
    friend ExactInteger operator<< (const ExactInteger& u, size_t v);
    friend ExactInteger operator/ (const ExactInteger& u, const ExactInteger& v);
    friend ExactInteger operator>> (const ExactInteger& u, size_t v);
    friend ExactInteger operator* (const ExactInteger& u, const ExactInteger& v);
    friend ExactInteger operator^ (const ExactInteger& u, const ExactInteger& v);
    friend ExactInteger operator| (const ExactInteger& u, const ExactInteger& v);
    friend bool operator< (const ExactInteger& u, const ExactInteger& v);
    friend bool operator> (const ExactInteger& u, const ExactInteger& v);
    friend bool operator<= (const ExactInteger& u, const ExactInteger& v);
    friend bool operator>= (const ExactInteger& u, const ExactInteger& v);
    friend bool operator== (const ExactInteger& u, const ExactInteger& v);
    friend bool operator!= (const ExactInteger& u, const ExactInteger& v);

    friend std::ostream& operator<< (std::ostream& os, const ExactInteger& u);
    friend std::istream& operator>> (std::istream& is, ExactInteger& u);

    Digit to_uint() const;
    std::string to_string() const;
    ExactInteger gcd(const ExactInteger& other) const;

private:
    std::vector<Digit> digits;

    void trim();

    friend QDebug operator<<(QDebug d, const ExactInteger &p);
};

#ifdef USE_QT
inline Q_CORE_EXPORT QDebug operator<<(QDebug d, const ExactInteger &p)
{
    d.space() << "ExactInteger(";
    d.nospace() << p.to_string();
    return d.nospace() << ")";
}
#endif // USE_QT


inline ExactInteger operator+ (const ExactInteger& u, const ExactInteger& v)
{
    ExactInteger w(u);
    w += v;
    return w;
}

inline ExactInteger operator/ (const ExactInteger& u, const ExactInteger& v)
{
    ExactInteger q, r;
    u.divide(v, q, r);
    return q;
}

inline ExactInteger operator% (const ExactInteger& u, const ExactInteger& v)
{
    ExactInteger q, r;
    u.divide(v, q, r);
    return r;
}

inline ExactInteger operator- (const ExactInteger& u, const ExactInteger& v)
{
    ExactInteger w(u);
    w -= v;
    return w;
}

inline ExactInteger operator* (const ExactInteger& u, const ExactInteger& v)
{
    const size_t m = u.digits.size();
    const size_t n = v.digits.size();
    ExactInteger w;
    w.digits.resize(m + n, 0);
    for (size_t j = 0; j < n; ++j)
    {
        ExactInteger::Wigit k = 0;
        for (size_t i = 0; i < m; ++i)
        {
            k += static_cast<ExactInteger::Wigit>(u.digits[i]) * v.digits[j] +
                 w.digits[i + j];
            w.digits[i + j] = static_cast<ExactInteger::Digit>(k);
            k >>= ExactInteger::BITS;
        }
        w.digits[j + m] = static_cast<ExactInteger::Digit>(k);
    }
    w.trim();
    return w;
}

inline ExactInteger operator<< (const ExactInteger& u, size_t v)
{
    ExactInteger w(u);
    w <<= v;
    return w;
}

inline ExactInteger operator>> (const ExactInteger& u, size_t v)
{
    ExactInteger w(u);
    w >>= v;
    return w;
}

inline ExactInteger operator& (const ExactInteger& u, const ExactInteger& v)
{
    ExactInteger w(u);
    w &= v;
    return w;
}

inline ExactInteger operator^ (const ExactInteger& u, const ExactInteger& v)
{
    ExactInteger w(u);
    w ^= v;
    return w;
}

inline ExactInteger operator| (const ExactInteger& u, const ExactInteger& v)
{
    ExactInteger w(u);
    w |= v;
    return w;
}

inline bool operator< (const ExactInteger& u, const ExactInteger& v)
{
    const size_t m = u.digits.size();
    size_t n = v.digits.size();
    if (m != n)
    {
        return (m < n);
    }
    for (--n; n != 0 && u.digits[n] == v.digits[n]; --n);
    return (u.digits[n] < v.digits[n]);
}

inline bool operator> (const ExactInteger& u, const ExactInteger& v)
{
    return (v < u);
}

inline bool operator<= (const ExactInteger& u, const ExactInteger& v)
{
    return !(v < u);
}

inline std::ostream& operator<< (std::ostream& os, const ExactInteger& u)
{
    os << u.to_string(); return os;
}

inline std::istream& operator>> (std::istream& is, ExactInteger& u)
{
    char digit = '\0';
    is >> digit;
    if (is.good() && std::isdigit(digit))
    {
        u = digit - '0';
        while (std::isdigit(is.peek()))
        {
            is >> digit;
            u = 10 * u + (digit - '0');
        }
    }
    else
    {
        is.setstate(std::ios_base::failbit);
    }
    return is;
}

inline bool operator>= (const ExactInteger& u, const ExactInteger& v)
{
    return !(u < v);
}

inline bool operator== (const ExactInteger& u, const ExactInteger& v)
{
    return (u.digits == v.digits);
}

inline bool operator!= (const ExactInteger& u, const ExactInteger& v)
{
    return (u.digits != v.digits);
}
