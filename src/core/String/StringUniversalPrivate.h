#pragma once

#include <utility>

#include "core/ByteArray/WByteArray.h"
#include "core/WListFast.h"
#include "core/WVector.h"

#ifdef USE_QT
# error "Qt in use but import wronge class"
#endif

class WString {
private:
    WByteArray _data;
    void test() const {};
public:
    WString () = default;
    WString (const char *string);
    WString (WByteArray str);
    WString (WString &&other) noexcept = default;
    WString (const WString &other) noexcept = default;
    WString (const std::string &other);

    ~WString() = default;

    WString toUpper() const;
    WString lower() const;

    [[nodiscard]]
    auto isEmpty() const -> bool;

    [[nodiscard]]
    auto size() const -> unsigned long;

    [[nodiscard]]
    auto toUtf8() const -> const WByteArray&;

    [[nodiscard]]
    auto split(char character) const -> WListFast<WString>;

    auto at(unsigned long index) const -> char;
    auto charAt(int index) const -> char;

    auto indexOf(const WString &other) const -> int;
    auto indexOf(char character) const -> int;
    auto lastIndexOf(char character) const -> int;
    auto lastIndexOf(const char *data) const -> int;
    void replace(char oldChar, char newChar);
    auto mid(int from, int to = -1) const -> WString;

    void insert(const WByteArray &other, int index);
    void insert(const WString &other, int index);

    void reserve(int numberOfChar);
    auto remove(int index) -> void;
    auto remove(int index) const -> WString;

    static auto number(int number) -> WString;
    static auto fromUtf8(const char *data, int size = -1) -> WString;

    auto arg(const WString &arg) const -> WString;
    auto arg(int a) const -> WString;

    bool operator==(const WString &other) const;
    bool operator!=(const WString &other) const;
    auto operator+(const WString &other) -> WString;
    auto operator+=(const WString &other) -> WString&;
    auto operator+=(char c) -> WString&;
    auto operator[](int i) -> char &;
    auto operator[](int i) const -> char;

    static auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<int, WString>;
    static auto write (WritableAbstract &writable, const WString &data) -> int;

    using Iterator = WByteArray::iterator;
    using ConstIterator = WByteArray::const_iterator;

    Iterator begin() noexcept { return _data.begin(); };
    Iterator end()   noexcept { return _data.end(); };

    [[nodiscard]] auto constBegin() const noexcept  { return _data.constBegin(); }
    [[nodiscard]] auto constEnd()   const noexcept  { return _data.constEnd(); }
    [[nodiscard]] auto cBegin() const noexcept      { return _data.constBegin(); }
    [[nodiscard]] auto cEnd()   const noexcept      { return _data.constEnd(); }
    [[nodiscard]] auto begin() const noexcept       { return _data.constBegin(); }
    [[nodiscard]] auto end()   const noexcept       { return _data.constEnd(); }

    friend bool operator<(const WString &first, const WString &second);

    auto append(const char *data, long size) -> WString&;

    auto toStdString() const noexcept -> std::string;

    auto operator>(const WString &other) const noexcept -> bool;
    auto operator>=(const WString &other) const noexcept -> bool;
    auto operator<=(const WString &other) const noexcept -> bool;
    auto operator<(const WString &other) const noexcept -> bool;

    auto operator=(const WString &other) noexcept -> WString& = default;
    auto operator=(WString &&other) noexcept -> WString& = default;
};

inline auto WString::size() const -> unsigned long
{
    return this->_data.size();
}

inline auto WString::toUtf8() const -> const WByteArray &
{
    return this->_data;
}

inline auto WString::split(char character) const -> WListFast<WString>
{
    WListFast<WString> res {};
    WString tmp {};

    for (const auto c: *this) {
        if (c == character) {
            res.append(tmp);
            tmp = "";
            continue;
        }

        tmp += c;
    }

    return res;
}

inline auto WString::indexOf(const WString &other) const -> int
{
    W_ASSERT(other.size() > 0);

    for (unsigned long i = 0; i < size(); i++) {
        if (other.at(i) == this->at(i)) {
            bool find = false;
            unsigned long k;

            for (k = 0; k < other.size() && i + k < size(); k++) {
                if (at(i + k) != other.at(k)) {
                    find = false;
                    break;
                }
                find = true;
            }

            if (not find) {
                // we reach a character different
                continue;
            }

            if (k == other.size()) {
                // we reach the end of other
                return i;
            }
        }
    }
    return -1;
}

inline auto WString::indexOf(char character) const -> int
{
    for (int i = 0; i < size(); i++) {
        if (at(i) == character) {
            return i;
        }
    }
    return -1;
}

inline auto WString::lastIndexOf(const char *str) const -> int
{
    const auto s = strlen(str);
    for (int i = size() - 1 - s; i >= 0; i--) {
        if (strcmp(this->_data.constData(), str) == 0)
            return i;
    }
    return -1;
}

inline int WString::lastIndexOf(char character) const
{
    for (long i = size() - 1; i >= 0; i --) {
        if (at(i) == character) {
            return i;
        }
    }

    return -1;
}

inline void WString::replace(char oldChar, char newChar)
{
    for (auto &ref: *this) {
        if (ref == oldChar) {
            ref = newChar;
        }
    }
}

inline char &WString::operator[](int i)
{
    return this->_data[i];
}

inline WString WString::mid(int from, int to) const
{
    if (to == -1)
        to = size();
    return _data.mid(from, to);
}

inline WString WString::number(int number)
{
    WString res;
    const auto str = std::to_string(number);

    res._data.append(
            str.c_str(),
            str.size()
    );

    return res;
}

inline char WString::at(unsigned long index) const
{
    return _data.at(index);
}

inline WString WString::fromUtf8(const char *data, int size)
{
    WString res{};
    res._data.append(data, size);
    return res;
}

inline auto WString::arg(const WString &arg) const -> WString
{
    WString res {this->_data};
    //int index = 1;
    res._data.reserve(arg.size());

    auto s = size();

    for (int i = 0; i < s - 1; i++) {
        //W_ASSERT(index < 9);

        const auto current = at(i);
        if (current != '%')
            continue;

        const auto next = at(i + 1);
        if (next >= '1' && next <= '9') {
            res.remove(i);
            res.remove(i + 1);

            s = s - 2 + arg.size();

            res.insert(arg._data, i);
        }
    }

    return res;
}

inline auto WString::operator!=(const WString &other) const -> bool
{
    return !(*this == other);
}

inline auto operator<(const WString &first, const WString &second) -> bool
{
    unsigned long i;

    if (first.size() < second.size())
        return true;

    auto b1 = first.begin(), b2 = second.begin();
    auto e1 = first.end(), e2 = second.end();

    for (; b1 != e1 and b2 != e2; b1 ++, b2 ++) {
        if (*b1 == *b2)
            continue;
        if (*b1 > *b2)
            return false;
        return true;
    }

    return false;
}

inline auto WString::append(const char *data, long size) -> WString&
{
    this->_data.append(data, size);
    return *this;
}

inline WString::WString(const char *string)
    : _data(string)
{

}

inline WString::WString(const std::string &other)
    : _data(other.c_str(), other.size())
{

}

inline auto WString::toUpper() const -> WString
{
    WString result(*this);
    for (auto &c: result)
        if (c >= 'a' and c <= 'z')
            c += 'A' - 'a';
    return result;
}

inline void WString::insert(const WByteArray &other, int index)
{
    _data.insert(other, index);
}

inline void WString::reserve(int numberOfChar)
{
    _data.reserve(numberOfChar);
}

inline auto WString::remove(int index) -> void
{
    _data.remove(index);
}

inline auto WString::operator==(const WString &other) const -> bool
{
    return this->_data == other._data;
}

inline WString WString::operator+(const WString &other)
{
    WString result;
    result.reserve(this->size() + other.size());
    result
        .append(_data.constData(), _data.size())
        .append(other._data.constData(), _data.size());
    return result;
}

inline auto WString::operator+=(const WString &other) -> WString&
{
    _data.append(other._data);
    return *this;
}

inline auto WString::operator+=(char c) -> WString&
{
    _data.append(c);
    return *this;
}

inline auto WString::arg(int a) const -> WString
{
    WString result (*this);
    WString number = WString::number(a);
    const auto index = indexOf('%');

    if (index < 0)
        return {};

    if (index + 1 == size())
        return *this;

    if (number.size())
        result[index] = number[0];
    if (number.size() > 1)
        result[index + 1] = number[1];
    result.insert(number.mid(2, number.size()), index + 2);

    return result;
}

inline void WString::insert(const WString &other, int index)
{
    return insert(other._data, index);
}

inline WString::WString(WByteArray str)
    : _data(std::move(str))
{

}

inline bool WString::isEmpty() const
{
    return this->_data.isEmpty();
}

inline auto WString::operator[](int i) const -> char
{
    return this->_data[i];
}

inline auto WString::toStdString() const noexcept -> std::string
{
    return this->_data.toStdString();
}

inline auto WString::remove(int index) const -> WString
{
    WString result (*this);
    result.remove(index);
    return result;
}

inline auto WString::charAt(int index) const -> char
{
    return at(index);
}

inline WString operator+(const char *s1, const WString &s2)
{ WString t = WString::fromUtf8(s1); t += s2; return t; }

inline std::ostream& operator<<(std::ostream& os, const WString& dt)
{
    os << dt.toUtf8();
    return os;
}
