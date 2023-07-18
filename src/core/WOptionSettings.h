#pragma once

#include "core/ByteArray/WByteArray.h"
#include "WVariant.h"
#include "core/WMap.h"
#include "core/String/WString.h"

#ifdef USE_QT
# include <QSettings>
#endif // USE_QT

// TODO: make this .h file more portable
class WOptionSettings {
private:
#ifdef USE_QT
    QSettings *settings;
#else
    static constexpr auto nameFileConfiguration = "writernote_configuration.txt";
    WMap<WString, WByteArray, WStringHash> _map;
#endif // USE_QT
public:
    WOptionSettings() = default;
    void begin();
    void save();

    void remove(const WByteArray &key);

    auto value(const WByteArray &key, const WVariant &defaultMessage) -> WVariant;
    void setValue(const WByteArray &key, const WVariant &value);
};

