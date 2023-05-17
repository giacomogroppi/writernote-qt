//
// Created by Giacomo Groppi on 30/04/23.
//

#ifndef WRITERNOTE_WSTRING_H
#define WRITERNOTE_WSTRING_H


#include "WByteArray.h"
#include "WListFast.h"

class WString {
private:
public:
    WString() = default;
    WString (const char *);
    WString(const WByteArray &str);
    WString (const WString &other);
    WString(const std::string &other);

    WString upper() const;
    WString lower() const;

    int size() const;
    int legth() const;

    const WByteArray &toUtf8() const;

    WListFast<WString> split(char character) const;

    int indexOf(const WString &other) const;
    int indexOf(char caracter) const;
    int lastIndexOf(char caracter) const;
    int lastIndexOf(const char *data) const;
    void replace(char oldChar, char newChar);
    WString mid(int from, int to) const;

    static WString number(int number);
    static WString fromUtf8(const char *data, int size = -1);

    WString arg(const WString &arg) const;

    bool operator==(const WString &other) const;
    WString &operator=(const WString &other);
    WString operator+(const WString &other);
    WString &operator+=(const WString &other);
};

inline WString operator+(const char *s1, const WString &s2)
{ WString t = WString::fromUtf8(s1); t += s2; return t; }

#endif //WRITERNOTE_WSTRING_H
