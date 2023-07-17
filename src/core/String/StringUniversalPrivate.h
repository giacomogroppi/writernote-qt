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
    WString() = default;
    WString (const char *string);
    WString(WByteArray str);
    WString(WString &&other) noexcept = default;
    WString (const WString &other) = default;
    WString(const std::string &other);

    WString toUpper() const;
    WString lower() const;

    [[nodiscard]]
    auto isEmpty() const -> bool;

    [[nodiscard]]
    auto size() const -> int;

    [[nodiscard]]
    auto toUtf8() const -> const WByteArray&;

    [[nodiscard]]
    auto split(char character) const -> WListFast<WString>;

    char at(int index) const;

    int indexOf(const WString &other) const;
    int indexOf(char character) const;
    int lastIndexOf(char character) const;
    int lastIndexOf(const char *data) const;
    void replace(char oldChar, char newChar);
    WString mid(int from, int to = -1) const;

    void insert(const WByteArray &other, int index);
    void insert(const WString &other, int index);

    void reserve(int numberOfChar);
    auto remove(int index) -> void;
    auto remove(int index) const -> WString;

    static WString number(int number);
    static WString fromUtf8(const char *data, int size = -1);

    auto arg(const WString &arg) const -> WString;
    auto arg(int a) const -> WString;

    bool operator==(const WString &other) const;
    bool operator!=(const WString &other) const;
    auto operator=(const WString &other) -> WString &;
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

    WString& append(const char *data, int size);

    std::string toStdString() const noexcept;
};

inline int WString::size() const
{
    return this->_data.size();
}

inline const WByteArray &WString::toUtf8() const
{
    return this->_data;
}

inline WListFast<WString> WString::split(char character) const
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

inline int WString::indexOf(const WString &other) const
{
    W_ASSERT(other.size() > 0);

    // TODO optimize
    for (int i = 0; i < size(); i++) {
        if (other.at(i) == this->at(i)) {
            bool find = false;
            int k;
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

inline int WString::indexOf(char character) const
{
    for (int i = 0; i < size(); i++) {
        if (at(i) == character) {
            return i;
        }
    }
    return -1;
}

inline int WString::lastIndexOf(char character) const
{
    for (int i = size() - 1; i >= 0; i --) {
        if (at(i) == character) {
            return i;
        }
    }

    return -1;
}

inline void WString::replace(char oldChar, char newChar)
{
    for (int i = 0; i < this->size(); i++) {
        if (at(i) == oldChar) {
            this->operator[](i) = newChar;
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

inline char WString::at(int index) const
{
    return _data.at(index);
}

inline WString WString::fromUtf8(const char *data, int size)
{
    WString res{};
    res._data.append(data, size);
    return res;
}

inline WString WString::arg(const WString &arg) const
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

inline bool WString::operator!=(const WString &other) const
{
    return !(*this == other);
}

inline bool operator<(const WString &first, const WString &second)
{
    int i;
    for (i = 0; i < first.size() and i < second.size(); i++) {
        if (first.at(i) > second.at(i))
            return false;
    }
    if (first.size() < second.size())
        return true;
    return false;
}

inline WString& WString::append(const char *data, int size)
{
    this->_data.append(data, size);
    return *this;
}

inline WString::WString(const char *string)
    : _data(string)
{

}

WString::WString(const std::string &other)
    : _data(other.c_str(), other.size())
{

}

inline WString WString::toUpper() const
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

inline auto WString::operator=(const WString &other) -> WString&
{
    if (this == &other)
        return *this;
    this->_data = other._data;
    return *this;
}

inline bool WString::operator==(const WString &other) const
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

inline WString &WString::operator+=(const WString &other)
{
    _data.append(other._data);
    return *this;
}

inline WString &WString::operator+=(char c)
{
    _data.append(c);
    return *this;
}

inline WString WString::arg(int a) const
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

char WString::operator[](int i) const
{
    return this->_data[i];
}

inline std::string WString::toStdString() const noexcept
{
    return this->_data.toStdString();
}

inline auto WString::remove(int index) const -> WString
{
    WString result (*this);
    result.remove(index);
    return result;
}

inline WString operator+(const char *s1, const WString &s2)
{ WString t = WString::fromUtf8(s1); t += s2; return t; }
