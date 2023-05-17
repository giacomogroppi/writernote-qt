//
// Created by Giacomo Groppi on 16/05/23.
//

#ifndef WRITERNOTE_WVARIANT_H
#define WRITERNOTE_WVARIANT_H

#include "WListFast.h"
#include "WList.h"
#include "WVector.h"
#include "WByteArray.h"
#include "WString.h"

class WVariant {
public:
    WVariant(const WVariant &other);
    WVariant(const WByteArray &data);
    WVariant(int data);

    std::pair<int, bool> toInt();
    std::pair<WString, bool> toString();
    std::pair<WByteArray, bool> &toByteArray();

    template <class T>
    std::pair<WListFast<T>, T> toWListFast();

    template <class T>
    std::pair<WList<T>, T> toWList();

    template <class T>
    std::pair<WVector<T>, T> toWVector();
};


#endif //WRITERNOTE_WVARIANT_H
