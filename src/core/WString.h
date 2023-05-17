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

    WString upper() const;
    WString lower() const;

    const WByteArray &toUtf8() const;

    WListFast<WString> split(char character);

    int lastIndexOf(char caracter) const;
    int lastIndexOf(const char *data) const;
    WString mid(int from, int to) const;

    static WString number(int number);
    static WString fromUtf8(const char *data, int size = -1);

    WString arg(const WString &arg) const;

    bool operator==(const WString &other) const;
    WString &operator=(const WString &other);
    WString operator+(const WString &other);
};


#endif //WRITERNOTE_WSTRING_H
