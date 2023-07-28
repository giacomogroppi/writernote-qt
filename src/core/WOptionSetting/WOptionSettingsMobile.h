#pragma once

#include "core/ByteArray/WByteArray.h"
#include "WVariant.h"
#include "core/WMap.h"
#include "core/String/WString.h"

class WOptionSettings
{
private:
    static constexpr auto nameFileConfiguration = "writernote_configuration.txt";
    WMap<WString, WByteArray, WStringHash> _map;
    
    static auto getPathWritable() -> WString;
public:
    WOptionSettings() = default;
    auto begin() -> bool;
    auto save() const -> bool;

    void remove(const WByteArray &key);

    auto value(const WByteArray &key, const WVariant &defaultMessage) -> WVariant;
    void setValue(const WByteArray &key, const WVariant &value);
};
