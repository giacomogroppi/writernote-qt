#include "WOptionSettings.h"
#include "WFile.h"

#if is_ios || is_android || (is_mac && !USE_QT)

void WOptionSettings::remove(const WByteArray &key)
{
    _map.remove(key);
}

auto WOptionSettings::value(const WByteArray &key, const WVariant &defaultMessage) -> WVariant
{
    if (this->_map.isPresent(key))
        return _map.get(key);
    return defaultMessage;
}

void WOptionSettings::setValue(const WByteArray &key, const WVariant &value)
{
    this->_map.set(key, value.toRaw());
}

auto WOptionSettings::begin() -> bool
{
    WFile file (WOptionSettings::getPathWritable().toUtf8(), WFile::WFileReadOnly);
    
    if (not file.isValid())
        return false;
    
    const auto [status, version] = VersionFileController::load (file);
    
    if (status < 0)
        return false;
    
    auto [res, data] = WMap<WString, WByteArray, WStringHash>::load (version, file);
}

#endif
