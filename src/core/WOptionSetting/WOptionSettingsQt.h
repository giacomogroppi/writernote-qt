#pragma once

#ifdef USE_QT

#include "core/ByteArray/WByteArray.h"
#include "core/WVariant.h"
#include "core/WMap.h"
#include "core/String/WString.h"

#include <QSettings>

class WOptionSettings
{
private:
    QSettings *settings;
public:
    WOptionSettings() = default;
    void begin();
    void save();

    void remove(const WByteArray &key);

    auto value(const WByteArray &key, const WVariant &defaultMessage) -> WVariant;
    void setValue(const WByteArray &key, const WVariant &value);

    /**
     * This is the name used for WOptionSettings for identify the
     * position in which the application should save.
    */
    static constexpr auto namePathSaving = "path_saving";
};

#endif // USE_QT
