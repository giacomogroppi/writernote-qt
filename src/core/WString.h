//
// Created by Giacomo Groppi on 30/04/23.
//

#ifndef WRITERNOTE_WSTRING_H
#define WRITERNOTE_WSTRING_H


#include "WByteArray.h"

class WString {
private:
public:
    WString() = default;
    WString (const char *);
    WString(const WByteArray &str);
    WString (const WString &other);

    WString upper() const;
    WString lower() const;

    bool operator==(const WString &other) const;
    WString &operator=(const WString &other);
};


#endif //WRITERNOTE_WSTRING_H
