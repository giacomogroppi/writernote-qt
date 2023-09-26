#include "WOptionSettings.h"
#include "core/WFile.h"

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
    WFile file (WOptionSettings::getPathWritable() / WOptionSettings::nameFileConfiguration,
                WFile::WFileReadOnly);
    
    if (not file.isValid())
        return false;
    
    const auto [status, version] = VersionFileController::load (file);
    
    if (status < 0)
        return false;
    
    auto [res, data] = WMap<WString, WByteArray, WStringHash>::load (version, file);
    
    if (res < 0)
        return false;
    
    this->_map = std::move(data);
    
    return true;
}

auto WOptionSettings::save() const -> bool
{
    WFile file (WOptionSettings::getPathWritable() / + WOptionSettings::nameFileConfiguration,
                WFile::WFileWrite);
    
    if (not file.isValid())
        return false;
    
    const auto res = VersionFileController::write (file);
    
    if (res < 0) return false;
    
    if (WMap<WString, WByteArray, WStringHash>::write (file, _map) < 0)
        return false;
    
    return true;
}

#endif
