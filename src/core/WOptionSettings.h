#pragma once

#include "core/ByteArray/WByteArray.h"
#include "WVariant.h"

#ifdef USE_QT
# include <QSettings>
#endif // USE_QT

class WOptionSettings {
private:
#ifdef USE_QT
    QSettings *settings;
#endif // USE_QT
public:
    WOptionSettings() = default;
    void begin();
    void save();

    void remove(const WByteArray &key);

    WVariant value(const WByteArray &key, const WVariant &defaultMessage);
    void setValue(const WByteArray &key, const WVariant &value);
};


