#pragma once

#ifdef USE_QT

#include "core/ByteArray/WByteArray.h"
#include "core/WVariant.h"
#include "core/WMap.h"
#include "core/String/WString.h"

#include <QSettings>

class WOptionSettings {
private:
    QSettings *settings;
public:
    WOptionSettings() = default;
    void begin();
    void save();

    void remove(const WByteArray &key);

    auto value(const WByteArray &key, const WVariant &defaultMessage) -> WVariant;
    void setValue(const WByteArray &key, const WVariant &value);
};

#endif // USE_QT
