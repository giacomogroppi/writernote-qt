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

#ifndef USE_QT
class WVariant {
public:
    WVariant(const WVariant &other);
    WVariant(const WByteArray &data);
    WVariant(int data);

    int toInt();
    WString toString();
    WByteArray &toByteArray();

    template <class T>
    WListFast<T> toWListFast();

    template <class T>
    WList<T> toWList();

    template <class T>
    WVector<T> toWVector();
};
#else
using WVariant = QVariant;
#endif // USE_QT


#endif //WRITERNOTE_WVARIANT_H
