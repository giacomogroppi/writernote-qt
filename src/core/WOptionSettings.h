#pragma once

#include "core/ByteArray/WByteArray.h"
#include "WVariant.h"
#include "core/WMap.h"

#ifdef USE_QT
# include <QSettings>
#endif // USE_QT

// TODO: make this .h file more portable
class WOptionSettings {
private:
#ifdef USE_QT
    QSettings *settings;
#else
    WMap<WString, WByteArray> _map;
#endif // USE_QT
public:
    WOptionSettings() = default;
    void begin();
    void save();

    void remove(const WByteArray &key);

    WVariant value(const WByteArray &key, const WVariant &defaultMessage);
    void setValue(const WByteArray &key, const WVariant &value);
};


void prova (){
    std::unordered_map<WString, int> d;
}