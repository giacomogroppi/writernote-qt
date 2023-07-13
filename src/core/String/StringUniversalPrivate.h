#pragma once

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
    WString (const char *);
    WString(const WByteArray &str);
    WString (const WString &other);
    WString(const std::string &other);

    WString (WString &&other) noexcept;

    WString toUpper() const;
    WString lower() const;

    int size() const;
    int legth() const;

    const WByteArray &toUtf8() const;

    WListFast<WString> split(char character) const;

    char at(int index) const;

    int indexOf(const WString &other) const;
    int indexOf(char character) const;
    int lastIndexOf(char character) const;
    int lastIndexOf(const char *data) const;
    void replace(char oldChar, char newChar);
    WString mid(int from, int to) const;

    void insert(const WByteArray &other, int index);

    void reserve(int numberOfChar);
    void remove(int index);

    static WString number(int number);
    static WString fromUtf8(const char *data, int size = -1);

    WString arg(const WString &arg) const;
    WString arg(int a) const;

    bool operator==(const WString &other) const;
    bool operator!=(const WString &other) const;
    WString &operator=(const WString &other);
    WString operator+(const WString &other);
    WString &operator+=(const WString &other);
    WString &operator+=(char c);
    char &operator[](int i);

    static auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> std::pair<int, WString>;
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

    void append(const char *data, int size);
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

inline void WString::append(const char *data, int size)
{
    this->_data.append(data, size);
}

inline WString operator+(const char *s1, const WString &s2)
{ WString t = WString::fromUtf8(s1); t += s2; return t; }
