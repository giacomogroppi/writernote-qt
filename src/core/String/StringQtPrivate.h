#pragma once

#ifndef USE_QT
# error "Qt not define but import adapter"
#endif // USE_Qt

#include <QString>

#include "VersionFileController.h"
#include "Writable.h"
#include "utils/slash/slash.h"

class WString: public QString {
public:
    WString() = default;
    WString (const WString &other) = default;
    WString (WString &&other) noexcept
            : QString(std::move (other)) {};

    WString (char c) noexcept: QString() { this->append(&c, 1); }

    WString (const QByteArray &other) noexcept
        : QString (other) {};

    WString (const char *data) noexcept
            : QString (data) {};

    WString (const QString &other) noexcept
            : QString(other) {};

    WString (QString &&other) noexcept
            : QString(std::move(other)) {}

    [[nodiscard]]
    auto canBeNameFile() const -> bool;

    [[nodiscard]]
    auto contains (std::initializer_list<char> listOfChar) const -> bool;

    [[nodiscard]]
    auto reverse () const -> WString
    {
        WString tmp;
        tmp.reserve(size());
        for (auto i = size() - 1; i >= 0; i--)
            tmp += at(i);
        return tmp;
    }

    [[nodiscard]]
    auto remove(char c) const -> WString
    {
        WString result;
        for (const auto caracter: *this) {
            if (caracter == c)
                continue;
               result.append(caracter);
        }
        return result;
    }

    static auto load (const VersionFileController& versionController,
                     ReadableAbstract &readable) -> WPair<Error, WString>;

    auto operator=(const WString &other) noexcept -> WString & = default;
    auto operator=(WString &&other) noexcept -> WString & = default;

    /**
     * @return &lt 0 iff writable fail
     * */
    static auto write (WritableAbstract &writable, const WString &str) -> Error;

    void append(const char *d, int size = -1)
    {
        if (size == -1)
            QString::append(d);
        else
            QString::append(QByteArray(d, size));
    }

    auto addSlashIfNecessary() const -> WString;

    void append (const QChar c);

    auto charAt(int i) const -> char;

    auto operator+(char c) const -> WString;
};

inline void WString::append(const QChar c)
{
    QString::append(c);
}

inline auto WString::charAt(int i) const -> char
{
    return at(i).toLatin1();
}

inline auto WString::addSlashIfNecessary() const -> WString
{
    if (isEmpty())
        return {slash::__slash()};
    if (at(size() - 1) != slash::__slash())
        return *this + slash::__slash();
    return *this;
}

inline auto WString::contains(std::initializer_list<char> listOfChar) const -> bool
{
    for (const auto &c: std::as_const(*this)) {
        for (const auto &letter: std::as_const(listOfChar)) {
            if (c == letter)
                return true;
        }
    }

    return false;
}

inline auto WString::canBeNameFile() const -> bool
{
    return not this->contains({'|', '\\', '/', ':', '.', '?', '!', '|', '.'});
}

inline auto WString::operator+(char c) const -> WString
{
    WString tmp(*this);

    tmp.append(c);

    return tmp;
}

