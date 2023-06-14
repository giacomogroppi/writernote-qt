//
// Created by Giacomo Groppi on 16/05/23.
//

#ifndef WRITERNOTE_WVARIANT_H
#define WRITERNOTE_WVARIANT_H

#include "core/WListFast.h"
#include "core/WList.h"
#include "core/WVector.h"
#include "core/WByteArray.h"
#include "core/String/WString.h"

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
