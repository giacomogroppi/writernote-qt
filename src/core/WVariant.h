#pragma once

#include "core/WListFast.h"
#include "core/WList.h"
#include "core/WVector.h"
#include "core/ByteArray/WByteArray.h"
#include "core/String/WString.h"

#ifndef USE_QT
class WVariant {
private:
    WByteArray _data;
public:
    WVariant (const WVariant &other) = default;
    WVariant (WVariant &&other) noexcept = default;

    explicit WVariant (const void *raw, int size);

    explicit WVariant (const WByteArray &data);
    explicit WVariant (int data);

    auto toInt() -> int;
    auto toString() -> WString;
    auto toByteArray() -> WByteArray;

    template <class T>
    auto toWListFast() -> WListFast<T>;

    template <class T>
    auto toWList() -> WList<T>;

    template <class T>
    auto toWVector() -> WVector<T>;

    auto operator=(const WVariant& other) noexcept -> WVariant& = default;
    auto operator=(WVariant &&other) noexcept -> WVariant& = default;
};
#else
using WVariant = QVariant;
#endif // USE_QT
